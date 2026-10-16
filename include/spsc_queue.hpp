
#pragma once

#include <array>
#include <atomic>
#include <cstddef>

#include "market_data_event.hpp"

template <std::size_t Capacity>
class SPSCQueue
{
    static_assert(Capacity > 0, "Queue capacity must be positive");

public:
    static constexpr std::size_t CAPACITY = Capacity;

    bool tryPush(const MarketDataEvent& event)
    {
        const std::size_t tail =
            tail_.load(std::memory_order_relaxed);

        const std::size_t head =
            head_.load(std::memory_order_acquire);

        if (tail - head == CAPACITY)
        {
            return false;
        }

        buffer_[tail % CAPACITY] = event;

        tail_.store(
            tail + 1,
            std::memory_order_release
        );

        return true;
    }

    bool tryPop(MarketDataEvent& event)
    {
        const std::size_t head =
            head_.load(std::memory_order_relaxed);

        const std::size_t tail =
            tail_.load(std::memory_order_acquire);

        if (head == tail)
        {
            return false;
        }

        event = buffer_[head % CAPACITY];

        head_.store(
            head + 1,
            std::memory_order_release
        );

        return true;
    }

private:
    std::array<MarketDataEvent, CAPACITY> buffer_{};

    std::atomic<std::size_t> head_{0};
    std::atomic<std::size_t> tail_{0};
};
