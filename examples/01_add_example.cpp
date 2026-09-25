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

    MarketDataEvent order4{
        EventType::ADD,
        Side::BUY,
        7001,
        20100,
        100
    };

    book.add(order1);
    book.add(order2);
    book.add(order3);
    book.add(order4);

    std::cout << "\n===== ORDER BOOK AFTER ADD =====\n";
    book.print();

    return 0;
}