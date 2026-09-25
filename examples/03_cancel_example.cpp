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

    std::cout << "\n===== BEFORE CANCEL =====\n";
    book.print();

    // ------------------------------------------------
    // Case 1:
    // Cancel one order from a PriceLevel that still
    // contains another order.
    // ------------------------------------------------

    std::cout << "\n===== CANCEL ORDER 5001 =====\n";

    bool cancelled = book.cancel(5001);

    std::cout << "Cancel result: "
              << (cancelled ? "success" : "failed")
              << '\n';

    book.print();

    // ------------------------------------------------
    // Case 2:
    // Order 5002 is alone at price 20095.
    // Cancelling it must also remove PriceLevel 20095.
    // ------------------------------------------------

    std::cout << "\n===== CANCEL ORDER 5002 =====\n";

    cancelled = book.cancel(5002);

    std::cout << "Cancel result: "
              << (cancelled ? "success" : "failed")
              << '\n';

    book.print();

    return 0;
}