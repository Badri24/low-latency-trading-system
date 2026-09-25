#include <iterator> 
#include <iostream> 
#include "order_book.hpp"


void OrderBook::add(const MarketDataEvent& event)
{
    //1. Convert MarketDataEvent -> Order
    Order order{
        event.order_id,
        event.side,
        event.price, 
        event.quantity
    }; 
    //2. Select BUY or SELL side 
    auto& levels = (event.side == Side::BUY) ? bids_ : asks_; 

    //3. Find existing PriceLevel or create a new one 
    auto& priceLevel = levels[event.price]; 

    //4. Add order at the end -> preserves FIFO
    priceLevel.orders.push_back(order); 

    //5. Get position of the newly inserted order 
    auto position = std::prev(priceLevel.orders.end());

    //6. Create a handle to find this order later 
    OrderHandle handle 
    {
        event.side,
        event.price,
        position
    }; 

    //7. Store handle indexed by OrderId
    orders_[event.order_id] = handle; 

}

bool OrderBook:: cancel (OrderId orderId)
{
    // 1. Find the order in the OrderId index 
    auto it = orders_.find(orderId); 

    if (it == orders_.end())
    {
        return false; 
    }

    //2. Retrieve information needed to locate the order
    const auto& handle = it->second; 

    //3. Select BUY or SELL side
    auto& levels = (handle.side == Side::BUY) ? bids_ : asks_; 

    //4. Find the corresponding PriceLevel 
    auto levelIt = levels.find(handle.price); 

    if (levelIt == levels.end())
    {
        return false; 
    }

    auto& priceLevel = levelIt-> second; 

    //5. Remove the order directly using its stored iterator 
    priceLevel.orders.erase(handle.position); 

    //6. Remove the PriceLevel if it became empty 
    if (priceLevel.orders.empty()) 
    {
        levels.erase(levelIt); 
    }

    //7. Remove the OrderId from index 
    orders_.erase(it); 

    return true; 
}

bool OrderBook::modifyQuantity(OrderId orderId, Quantity newQuantity)
{
    
    //1. Quantity 0 means cancel 
    if (newQuantity == 0)
    {
        return cancel(orderId); 
    }
    
    //2. Find the order 
    auto it = orders_.find(orderId); 

    if (it == orders_.end())
    {
        return false; 
    }

    //3. Get the handle
    auto& handle = it->second; 

    //4. Access the actual Order 
    auto& order = *(handle.position); 

    //5. Quantity reduction: keep FIFO priority 
    if(newQuantity <= order.quantity)
    {
        order.quantity = newQuantity; 
        return true; 
    }

    //6. Quatity increase: lose FIFO priority 
    auto& levels = (handle.side == Side::BUY) ? bids_ : asks_; 

    auto levelIt = levels.find(handle.price); 

    if (levelIt == levels.end())
    {
        return false; 
    }

    auto& priceLevel = levelIt-> second; 

    // Update qunatity 
    order.quantity = newQuantity; 

    // Move the order to the end of the list (Dans priceLevel.orders, déplace l'élément situé à handle.position vers la position end())
    priceLevel.orders.splice(
        priceLevel.orders.end(), 
        priceLevel.orders, 
        handle.position
    ); 

    // Update the stored iterator 
    handle.position = std::prev(priceLevel.orders.end()); 
    
    return true; 
}

bool OrderBook::modifyPrice( OrderId orderId, Price newPrice)
{
    //Find the order
    auto it = orders_.find(orderId);

    if (it==orders_.end())
    {
        return false; 
    }

    auto& handle = it->second; 

    // Same price: nothing to change 
    if (handle.price == newPrice)
    {
        return true; 
    }

    // Select BUY or SELL side 
    auto& levels = (handle.side == Side::BUY) ?  bids_ : asks_; 

    // retrive the old priceLevel 
    auto oldLevelIt = levels.find(handle.price);
    
    if (oldLevelIt == levels.end())
    {
        return false; 
    }

    auto& oldPriceLevel = oldLevelIt->second; 

    //Copy the order before removing it 
    Order order = *(handle.position); 

    //Update the price in the copy
    order.price = newPrice;

    //Remove the original order 
    oldPriceLevel.orders.erase(handle.position); 

    // Remove the old PriceLevel if it  became empty 
    if ( oldPriceLevel.orders.empty())
    {
        levels.erase(oldLevelIt); 
    }

    // Find or create the new PriceLevel
     auto & newPriceLevel = levels[newPrice]; 

     // Insert at the end: new FIFO priotity 
     newPriceLevel.orders.push_back(order);
     
     // Get the new position 
     auto newPosition = std:: prev(newPriceLevel.orders.end()); 

     // Update the OrderHandle 
     handle.price = newPrice; 
     handle.position = newPosition; 

     return true; 
}

    bool OrderBook:: trade(OrderId orderId, Quantity tradeQuantity)
    {
        if (tradeQuantity ==0 )
        {
            return false; 
        }

        auto it = orders_.find(orderId); 

        if ( it == orders_.end())
        {
            return false; 
        }

        auto& handle = it->second; 
        auto& order = *(handle.position); 

        if (tradeQuantity >order.quantity)
        {
            return false; 
        }

        if(tradeQuantity == order.quantity)
        {
            return cancel(orderId); 
        }

        order.quantity -= tradeQuantity; 

        return true; 

    }

Price OrderBook:: bestBid() const
{
    if (bids_.empty())
    {
        return 0; 
    }

    return std::prev(bids_.end())->first; 
}

Price OrderBook:: bestAsk() const
{
    if (asks_.empty())
    {
        return 0; 
    }

    return asks_.begin()->first; 
}

void OrderBook::print() const
{
    std::cout << "\n===== BIDS =====\n";

    for (const auto& [price, priceLevel] : bids_)
    {
        std::cout << "Price: " << price
                  << " | Orders: ";

        for (const auto& order : priceLevel.orders)
        {
            std::cout << "[ID=" << order.order_id
                      << ", Qty=" << order.quantity << "] ";
        }

        std::cout << '\n';
    }

    std::cout << "\n===== ASKS =====\n";

    for (const auto& [price, priceLevel] : asks_)
    {
        std::cout << "Price: " << price
                  << " | Orders: ";

        for (const auto& order : priceLevel.orders)
        {
            std::cout << "[ID=" << order.order_id
                      << ", Qty=" << order.quantity << "] ";
        }
  
        std::cout << '\n';
    }

    std::cout << "\n===== ORDERS INDEX =====\n";

    for (const auto& [orderId, handle] : orders_)
    {
        std::cout << "Order ID: " << orderId
                  << " | Side: "
                  << (handle.side == Side::BUY ? "BUY" : "SELL")
                  << " | Price: " << handle.price
                  << '\n';
    }
}