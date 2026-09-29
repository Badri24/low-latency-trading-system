#pragma once 

#include <cstddef>
#include <vector> 

#include "market_data_event.hpp"

class MarketFeedSimulator 
{
    public: 
        MarketFeedSimulator(); 

        bool hasNext() const;  // const member function 

        MarketDataEvent next(); 

    private: 
        std::vector <MarketDataEvent> events_; 
        std::size_t currentIndex_; 
 }; 