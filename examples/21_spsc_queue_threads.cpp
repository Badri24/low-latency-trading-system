
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <thread>

#include "spsc_queue.hpp"

constexpr std::size_t EVENT_COUNT = 1'000'000;
constexpr std::size_t QUEUE_CAPACITY = 1024;

int main()
{
    SPSCQueue<QUEUE_CAPACITY> queue;

    std::size_t receivedCount = 0;
    bool orderValid = true;
    bool payloadValid = true;

    std::size_t producerRetries = 0;
    std::size_t consumerRetries = 0;

    const auto start = std::chrono::steady_clock::now();

    // Producer thread
    std::thread producer([&]()
    {
        for (std::uint64_t id = 1; id <= EVENT_COUNT; ++id)
        {
            MarketDataEvent event{
                EventType::ADD,
                Side::BUY,
                id,
                20100 + static_cast<std::int64_t>(id),
                100
            };

            while (!queue.tryPush(event))
            {
                ++producerRetries;
                std::this_thread::yield();
            }
        }
    });

    // Consumer thread
    std::thread consumer([&]()
    {
        MarketDataEvent event{};

        for (std::uint64_t expectedId = 1; expectedId <= EVENT_COUNT; ++expectedId)
        {
            while (!queue.tryPop(event))
            {
                ++consumerRetries;
                std::this_thread::yield();
            }

            ++receivedCount;

            if (event.order_id != expectedId)
            {
                orderValid = false;
            }

            if (event.price != 20100 + static_cast<std::int64_t>(expectedId) || event.quantity != 100 || event.type != EventType::ADD || event.side != Side::BUY)
            {
                payloadValid = false;
            }
        }
    });

    producer.join();
    consumer.join();

    const auto end = std::chrono::steady_clock::now();

    const auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        );

    std::cout << std::boolalpha;

    std::cout << "Events sent: " << EVENT_COUNT << '\n';
    std::cout << "Events received: " << receivedCount << '\n';

    std::cout << "FIFO order valid: "
              << orderValid << '\n';

    std::cout << "Payload valid: "
              << payloadValid << '\n';

    std::cout << "Producer retries: "
              << producerRetries << '\n';

    std::cout << "Consumer retries: "
              << consumerRetries << '\n';

    std::cout << "Elapsed time: "
              << duration.count()
              << " us\n";

    const bool success =
        receivedCount == EVENT_COUNT
        && orderValid
        && payloadValid;

    std::cout << "Test passed: "
              << success << '\n';

    return success ? 0 : 1;
}
