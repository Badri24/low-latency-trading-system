
#include <atomic>
#include <iostream>
#include <thread>

#include "event_processor.hpp"
#include "market_feed_simulator.hpp"
#include "order_book.hpp"
#include "spsc_queue.hpp"

int main()
{
    // Our existing trading system components.
    OrderBook orderBook;
    MarketFeedSimulator simulator;
    EventProcessor processor(orderBook);

    // Fixed-size, single-producer/single-consumer ring buffer.
    SPSCQueue<1024> queue;

    // Indicates that the producer has finished generating events.
    std::atomic<bool> producerDone{false};

    // Counters are owned by their respective threads.
    std::size_t producedCount = 0;
    std::size_t consumedCount = 0;
    std::size_t failedCount = 0;

    // --------------------------------------------------
    // CONSUMER THREAD
    // --------------------------------------------------
    std::thread consumer(
        [&]()
        {
            MarketDataEvent event{};

            while (true)
{
    // Acquire completion status before checking the queue.
    const bool done =
        producerDone.load(std::memory_order_acquire);

    if (queue.tryPop(event))
    {
        ++consumedCount;

        const bool success = processor.process(event);

        if (!success)
        {
            ++failedCount;

            std::cout
                << "Failed to process event for OrderId="
                << event.order_id
                << '\n';
        }

        continue;
    }

    // A failed pop after observing completion means
    // there are no published events left to consume.
    if (done)
    {
        break;
    }

    std::this_thread::yield();
}
        }
    );

    // --------------------------------------------------
    // PRODUCER THREAD
    // --------------------------------------------------
    std::thread producer(
        [&]()
        {
            while (simulator.hasNext())
            {
                MarketDataEvent event = simulator.next();

                // Retry if the bounded queue is full.
                // Never silently discard a market event.
                while (!queue.tryPush(event))
                {
                    std::this_thread::yield();
                }

                ++producedCount;
            }

            // Publish completion after all events
            // have been successfully pushed.
            producerDone.store(
                true,
                std::memory_order_release
            );
        }
    );

    // Wait for the producer and consumer to finish.
    producer.join();
    consumer.join();

    // The main thread can safely read the counters
    // and OrderBook after both joins.
    std::cout << "\nPipeline statistics:\n";

    std::cout
        << "Events produced: " << producedCount << '\n'
        << "Events consumed: " << consumedCount << '\n'
        << "Processing failures: " << failedCount << '\n';

    const bool allDelivered = producedCount == consumedCount;

    std::cout
        << std::boolalpha
        << "All events delivered: " << allDelivered
        << '\n';

    std::cout << "\nFinal Order Book:\n";
    orderBook.print();

    return allDelivered ? 0 : 1;
}
