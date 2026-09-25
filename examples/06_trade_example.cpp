#include <iostream>

#include "order_book.hpp"

int main()
{
    OrderBook book;

    MarketDataEvent sellOrder{
        EventType::ADD,
        Side::SELL,
        6001,
        20105,
        200
    };

    book.add(sellOrder);

    std::cout << "\n===== INITIAL ORDER BOOK =====\n";
    book.print();


    // --------------------------------------------------------
    // Case 1: Zero quantity
    // --------------------------------------------------------

    std::cout
        << "\n===== INVALID TRADE: ZERO QUANTITY =====\n";

    bool traded = book.trade(6001, 0);

    std::cout
        << "Trade result: "
        << (traded ? "success" : "failed")
        << '\n';

    book.print();


    // --------------------------------------------------------
    // Case 2: Overfill
    // --------------------------------------------------------

    std::cout
        << "\n===== INVALID TRADE: OVERFILL =====\n";

    traded = book.trade(6001, 300);

    std::cout
        << "Trade result: "
        << (traded ? "success" : "failed")
        << '\n';

    book.print();

    // --------------------------------------------------------
    // Case 3: Unknown OrderId
    // --------------------------------------------------------

    std::cout
        << "\n===== INVALID TRADE: UNKNOWN ORDER =====\n";

    traded = book.trade(9999, 50);

    std::cout
        << "Trade result: "
        << (traded ? "success" : "failed")
        << '\n';

    book.print();

    // --------------------------------------------------------
    // Case 4: Partial fill
    // --------------------------------------------------------

    std::cout
        << "\n===== PARTIAL FILL: 50 SHARES =====\n";

    traded = book.trade(6001, 50);

    std::cout
        << "Trade result: "
        << (traded ? "success" : "failed")
        << '\n';

    book.print();

    // --------------------------------------------------------
    // Case 5: Full fill
    // --------------------------------------------------------

    std::cout
        << "\n===== FULL FILL: 150 SHARES =====\n";

    traded = book.trade(6001, 150);

    std::cout
        << "Trade result: "
        << (traded ? "success" : "failed")
        << '\n';

    book.print();

    return 0;
}