#include <iostream>
#include <stdexcept>

#include "market_feed_simulator.hpp"

const char* toString(EventType type)
{
    switch (type)
    {
        case EventType::ADD:
            return "ADD";

        case EventType::MODIFY:
            return "MODIFY";

        case EventType::CANCEL:
            return "CANCEL";

        case EventType::TRADE:
            return "TRADE";
    }

    return "UNKNOWN";
}

const char* toString(Side side)
{
    switch (side)
    {
        case Side::BUY:
            return "BUY";

        case Side::SELL:
            return "SELL";
    }

    return "UNKNOWN";
}

int main()
{
    MarketFeedSimulator simulator;

    while (simulator.hasNext())
    {
        MarketDataEvent event = simulator.next();

        std::cout
            << "Type=" <<toString(event.type)
            << " | Side=" << toString(event.side)
            << " | ID=" << event.order_id
            << " | Price=" << event.price
            << " | Qty=" << event.quantity
            << '\n';
    }

    std::cout << "\nTrying to read past end of feed...\n";

    try
    {
        simulator.next();
    }
    catch (const std::out_of_range& exception)
    {
        std::cout
            << "Caught expected exception: "
            << exception.what()
            << '\n';
    }

    return 0;
}