 #pragma once
 
 #include <cstdint>
 #include <map>
 #include <list>
 #include <unordered_map>

 #include "market_data_event.hpp"

using OrderId = std::uint64_t;
using Price = std::int64_t;
using Quantity = std::uint32_t;

struct Order
{
    OrderId order_id;
    Side side;
    Price price;
    Quantity quantity;
};

struct PriceLevel
 {
    std::list<Order> orders;
 }; 
 
 struct OrderHandle 
 {
    Side side; 
    Price price; 
    std::list<Order>:: iterator position;  
 }; 

 class OrderBook
 {
   public: 
   
      void add(const MarketDataEvent& event);

      bool cancel(OrderId orderId); 

      bool modifyQuantity(OrderId orderId, Quantity newQuantity);

      bool modifyPrice( OrderId orderId, Price newPrice); 

      bool trade(OrderId orderId, Quantity tradeQuantity); 

      Price bestBid() const; 
      Price bestAsk() const; 

      void print() const; 

    private: 
        std::map<Price, PriceLevel> bids_;  
        std::map<Price, PriceLevel> asks_; 

        std::unordered_map<OrderId , OrderHandle> orders_; 
 }; 



 
 




