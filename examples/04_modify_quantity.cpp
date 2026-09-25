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
        7001,
        20100,
        100
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

    std::cout << "\n===== INITIAL ORDER BOOK =====\n";
    book.print();

    // ------------------------------------------------
    // Case 1:
    // Reduce quantity.
    // The order keeps its FIFO priority.
    // ------------------------------------------------

    std::cout << "\n===== MODIFY 5001: 400 -> 150 =====\n";

    bool modified =
        book.modifyQuantity(5001, 150);

    std::cout << "Modify result: "
              << (modified ? "success" : "failed")
              << '\n';

    book.print();

    // Expected FIFO:
    //
    // 20100 -> [5001 Qty=150] -> [7001 Qty=100]


    // ------------------------------------------------
    // Case 2:
    // Increase quantity.
    // In our V1 rule, the order loses FIFO priority
    // and moves to the end of the PriceLevel.
    // ------------------------------------------------

    std::cout << "\n===== MODIFY 5001: 150 -> 600 =====\n";

    modified =
        book.modifyQuantity(5001, 600);

    std::cout << "Modify result: "
              << (modified ? "success" : "failed")
              << '\n';

    book.print();

    // Expected FIFO:
    //
    // 20100 -> [7001 Qty=100] -> [5001 Qty=600]


    // ------------------------------------------------
    // Case 3:
    // Quantity == 0 is treated as CANCEL.
    // ------------------------------------------------

    std::cout << "\n===== MODIFY 5001: 600 -> 0 =====\n";

    modified =
        book.modifyQuantity(5001, 0);

    std::cout << "Modify result: "
              << (modified ? "success" : "failed")
              << '\n';

    book.print();

    // Expected:
    //
    // 20100 -> [7001 Qty=100]
    //
    // Order 5001 must also disappear from orders_.

    return 0;
}