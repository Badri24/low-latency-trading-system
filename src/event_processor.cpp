#include "event_processor.hpp"


EventProcessor::EventProcessor(OrderBook& orderBook)
    : orderBook_(orderBook)
{
}

bool EventProcessor::process(const MarketDataEvent& event)
{
    switch(event.type)
    {
        case EventType::ADD: 
            orderBook_.add(event); 
            return true; 

        case EventType::MODIFY:
            return orderBook_.modifyQuantity(event.order_id, event.quantity); 

        case EventType::CANCEL: 
            return orderBook_.cancel(event.order_id); 

        case EventType::TRADE: 
            return orderBook_.trade(event.order_id, event.quantity); 
    }

    return false; 
} 