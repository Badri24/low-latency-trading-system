#include <iostream>
#include <thread>

#include "event_processor.hpp"
#include "market_feed_simulator.hpp"
#include "order_book.hpp"
#include "thread_safe_queue.hpp"

int main()
{
    OrderBook orderBook;
    MarketFeedSimulator simulator;
    ThreadSafeQueue queue;
    EventProcessor processor(orderBook);

    std::thread consumer(
    [&queue, &processor]()
    {
        MarketDataEvent event;

        while (queue.waitAndPop(event))
        {
            bool success = processor.process(event);

            if (!success)
            {
                std::cout
                    << "Failed to process event for OrderId="
                    << event.order_id
                    << '\n';
            }
        }
    }
);

    std::thread producer(
    [&simulator, &queue]()
    {
        while (simulator.hasNext())
        {
            MarketDataEvent event = simulator.next();

            queue.push(event);
        }

        queue.close();
    }
);

producer.join();
consumer.join();

std::cout << "\nFinal Order Book:\n";
orderBook.print();

    return 0;
}