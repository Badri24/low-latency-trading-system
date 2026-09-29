#pragma once 

#include "market_data_event.hpp"
#include "order_book.hpp"

class EventProcessor 
{
    public: 
        explicit EventProcessor(OrderBook& orderBook); 

        bool process(const MarketDataEvent& event); 

    private: 
        OrderBook& orderBook_; 
}; 