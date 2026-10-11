
#include <atomic>
#include <cstdint>
#include <iostream>
#include <thread>

struct MarketSnapshot
{
    std::int64_t bestBid;
    std::int64_t bestAsk;
    std::uint64_t sequenceNumber;
};

MarketSnapshot snapshot{};
std::atomic<bool> ready{false};

void producer()
{
    snapshot.bestBid = 20100;
    snapshot.bestAsk = 20105;
    snapshot.sequenceNumber = 42;

    // Publish all preceding writes.
    ready.store(true, std::memory_order_release);
}

void consumer()
{
    // Wait for the publication.
    while (!ready.load(std::memory_order_acquire))
    {
        std::this_thread::yield();
    }

    std::cout << "Best Bid: " << snapshot.bestBid << '\n';
    std::cout << "Best Ask: " << snapshot.bestAsk << '\n';
    std::cout << "Sequence: " << snapshot.sequenceNumber << '\n';
}

int main()
{
    std::thread consumerThread(consumer);
    std::thread producerThread(producer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
