#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

#include "market_data_event.hpp"

class ThreadSafeQueue
{
public:
    void push(const MarketDataEvent& event);

    bool waitAndPop(MarketDataEvent& event);

    void close();

private:
    std::queue<MarketDataEvent> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool closed_{false};
};