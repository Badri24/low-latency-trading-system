#include "thread_safe_queue.hpp"

void ThreadSafeQueue::push(const MarketDataEvent& event)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (closed_)
        {
            return;
        }

        queue_.push(event);
    }

    condition_.notify_one();  //Single Producer, Single Consumer 
}

void ThreadSafeQueue::close()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
    }

    condition_.notify_all();
}

bool ThreadSafeQueue::waitAndPop(MarketDataEvent& event)
{
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait(
        lock,
        [this]
        {
            return !queue_.empty() || closed_;
        }
    );

    if (queue_.empty())
    {
        return false;
    }

    event = queue_.front();
    queue_.pop();

    return true;
}
