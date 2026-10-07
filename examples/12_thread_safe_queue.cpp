#include <chrono>
#include <iostream>
#include <thread>

#include "thread_safe_queue.hpp"

int main()
{
    ThreadSafeQueue queue;

    std::thread consumer(
    [&queue]()
    {
        MarketDataEvent event;

        while (queue.waitAndPop(event))
        {
            std::cout
                << "Consumer received OrderId="
                << event.order_id
                << '\n';
        }

        std::cout << "Consumer finished\n";
    }
    );

    std::this_thread::sleep_for(
        std::chrono::seconds(2)
    ); 

    std::thread producer(
    [&queue]()
    {
        queue.push(
            {EventType::ADD, Side::BUY, 1001, 20100, 400}
        );

        queue.push(
            {EventType::ADD, Side::BUY, 1002, 20095, 600}
        );

        queue.push(
            {EventType::ADD, Side::SELL, 2001, 20105, 200}
        );

        queue.close();

        std::cout << "Producer finished\n";
    }
    );

    producer.join(); 
    consumer.join(); 

    return 0;
}