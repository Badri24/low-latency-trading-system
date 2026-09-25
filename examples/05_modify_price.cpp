#include <iostream>

#include "order_book.hpp"

int main()
{
    OrderBook book;
    
    MarketDataEvent order1{
        EventType:: ADD, 
        Side::BUY,
        5001,   //order id 
        20100,  //price in ticks $201.00
        400
    }; 

    MarketDataEvent order2{
        EventType :: ADD, 
        Side:: BUY, 
        5002, 
        20095, 
        600
    }; 

    MarketDataEvent order3{
        EventType::ADD,
        Side::SELL,
        6001,
        20105,   // $201.05
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

    std::cout << "\n===== BEFORE MODIFY PRICE =====\n";
    book.print();

    bool modified = book.modifyPrice(5001, 20095);

    std::cout << "Modify result: "
          << (modified ? "success" : "failed")
          << '\n';

    std::cout << "\n===== AFTER MODIFY PRICE =====\n";

    book.print();

    std::cout << "\n Best Bid: " <<book.bestBid() << '\n'; 
    std::cout << "\n Best Ask: " << book.bestAsk() << '\n'; 

    std::cout << "\n===== SECOND MODIFY PRICE =====\n";

    std::cout << "Modify order 7001: Price 20100 -> 20090\n";

    bool modified2 = book.modifyPrice(7001, 20090);

    std::cout << "Modify result: " << (modified2 ? "success" : "failed") << '\n';

    book.print();

    std::cout << "\n===== SAME PRICE MODIFY =====\n";

    std::cout << "Modify order 5001: Price 20095 -> 20095\n";

    bool samePriceModified = book.modifyPrice(5001, 20095);

    std::cout << "Modify result: " << (samePriceModified ? "success" : "failed") << '\n';

    book.print();

    std::cout << "\n===== MODIFY UNKNOWN ORDER =====\n";

    std::cout << "Modify order 9999: Price -> 20100\n";

    bool unknownModified = book.modifyPrice(9999, 20100);

    std::cout << "Modify result: " << (unknownModified ? "success" : "failed") << '\n';

    book.print();

    std::cout<< "\nBest Bid: " << book.bestBid() << '\n';

    std::cout << "Best Ask: " << book.bestAsk() << '\n';

    return 0;
}


