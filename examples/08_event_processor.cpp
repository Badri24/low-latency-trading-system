#include <iostream>

#include "event_processor.hpp"
#include "market_feed_simulator.hpp"
#include "order_book.hpp"

int main()
{
    OrderBook orderBook;
    MarketFeedSimulator simulator;
    EventProcessor processor(orderBook);

    while (simulator.hasNext())
    {
        MarketDataEvent event = simulator.next();

        bool success = processor.process(event);

        if (!success)
        {
            std::cout
                << "Failed to process event for OrderId="
                << event.order_id
                << '\n';
        }
    }

    std::cout << "\nFinal Order Book:\n";
    orderBook.print();

    return 0;
}