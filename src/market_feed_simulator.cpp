#include <stdexcept>

#include "market_feed_simulator.hpp"

MarketFeedSimulator::MarketFeedSimulator()
    : events_{
        {EventType::ADD, Side::BUY, 1001, 20100, 400},
        {EventType::ADD, Side::BUY, 1002, 20095, 600},
        {EventType::ADD, Side::SELL, 2001, 20105, 200},
        {EventType::ADD, Side::SELL, 2002, 20110, 300},
        {EventType::MODIFY, Side::BUY, 1001, 20100, 300},
        {EventType::TRADE, Side::SELL, 2001, 20105, 50},
        {EventType::CANCEL, Side::BUY, 1002, 20095, 0}
      },
      currentIndex_(0)
{
}

bool MarketFeedSimulator::hasNext() const 
{
    return currentIndex_ < events_.size(); 
}

MarketDataEvent MarketFeedSimulator:: next()
{
    if(!hasNext())
    {
        throw std::out_of_range(
            "No more market data events"
        ); 
    }

    return events_[currentIndex_ ++]; 
}