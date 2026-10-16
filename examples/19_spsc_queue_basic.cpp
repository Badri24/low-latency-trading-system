
#include <iostream>

#include "spsc_queue.hpp"

int main()
{
    SPSCQueue<1024> queue;

    MarketDataEvent event1{
        EventType::ADD,
        Side::BUY,
        1001,
        20100,
        400
    };

    MarketDataEvent event2{
        EventType::ADD,
        Side::SELL,
        2001,
        20105,
        200
    };

    bool pushed1 = queue.tryPush(event1);
    bool pushed2 = queue.tryPush(event2);

    std::cout << std::boolalpha;
    std::cout << "Push event1: " << pushed1 << '\n';
    std::cout << "Push event2: " << pushed2 << '\n';

    MarketDataEvent received{};

    while (queue.tryPop(received))
    {
        std::cout
            << "Received OrderId=" << received.order_id
            << " Price=" << received.price
            << " Quantity=" << received.quantity
            << '\n';
    }

    bool poppedFromEmpty = queue.tryPop(received);

    std::cout
        << "Pop from empty queue: "
        << poppedFromEmpty
        << '\n';

    return 0;
}
