#include <atomic>
#include <iostream>
#include <thread>

#include "market_data_event.hpp"

MarketDataEvent sharedEvent{};
std::atomic<bool> ready{false};

void producer()
{
    sharedEvent = {
        EventType::ADD,
        Side::BUY,
        1001,
        20100,
        400
    };

    ready.store(true, std::memory_order_release);
}

void consumer()
{
    while (!ready.load(std::memory_order_acquire))
    {
        // Wait until the event is published
    }

    std::cout << "Order ID: " << sharedEvent.order_id << '\n';
    std::cout << "Price: " << sharedEvent.price << '\n';
    std::cout << "Quantity: " << sharedEvent.quantity << '\n';
}

int main()
{
    std::thread consumerThread(consumer);
    std::thread producerThread(producer);

    producerThread.join();
    consumerThread.join();

    return 0;
}