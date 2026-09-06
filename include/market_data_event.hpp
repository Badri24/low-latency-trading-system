#include <cstdint> 
#pragma once 

/*
┌─────────────────────────────────────┐
│          MarketDataEvent            │
├─────────────────────────────────────┤
│ EventType     → event type          │
│ Side          → BUY / SELL          │
│ uint64_t      → order ID            │
│ int64_t       → price in ticks      │
│ uint32_t      → quantity            │
└─────────────────────────────────────┘
*/

enum class EventType
{
    ADD,
    MODIFY,
    CANCEL, 
    TRADE 
}; 

enum class Side 
{
    BUY,
    SELL
}; 

struct MarketDataEvent 
{
    EventType type; 
    Side side; 
    uint64_t order_id;
    int64_t price;  
    uint32_t quantity;
};

