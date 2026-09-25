#include <iostream>

#include "order_book.hpp"

int main()
{
    OrderBook book;

    MarketDataEvent order1{
        EventType::ADD,
        Side::BUY,
        5001,
        20100,
        400
    };

    MarketDataEvent order2{
        EventType::ADD,
        Side::BUY,
        5002,
        20095,
        600
    };

    MarketDataEvent order3{
        EventType::ADD,
        Side::SELL,
        6001,
        20105,
        200
    };

    book.add(order1);
    book.add(order2);
    book.add(order3);

    book.print();

    std::cout << "\nBest Bid: "
              << book.bestBid()
              << '\n';

    std::cout << "Best Ask: "
              << book.bestAsk()
              << '\n';

    return 0;
}