
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>

constexpr int ITERATIONS = 1'000'000;

// Shared counters
std::uint64_t mutexCounter = 0;
std::atomic<std::uint64_t> atomicCounter{0};

std::mutex counterMutex;

// Version 1: Mutex
void incrementWithMutex()
{
    for (int i = 0; i < ITERATIONS; ++i)
    {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++mutexCounter;
    }
}

// Version 2: Atomic
void incrementWithAtomic()
{
    for (int i = 0; i < ITERATIONS; ++i)
    {
        atomicCounter.fetch_add(1);
    }
}

template<typename Function>
double benchmark(Function function)
{
    const auto start = std::chrono::steady_clock::now();

    std::thread thread1(function);
    std::thread thread2(function);

    thread1.join();
    thread2.join();

    const auto end = std::chrono::steady_clock::now();

    return std::chrono::duration<double, std::milli>(
        end - start
    ).count();
}

int main()
{
    const double mutexTime = benchmark(incrementWithMutex);

    const double atomicTime = benchmark(incrementWithAtomic);

    std::cout << "Expected: " << 2 * ITERATIONS << '\n';

    std::cout << "\nMutex:\n";
    std::cout << "Counter: " << mutexCounter << '\n';
    std::cout << "Time: " << mutexTime << " ms\n";

    std::cout << "\nAtomic:\n";
    std::cout << "Counter: " << atomicCounter.load() << '\n';
    std::cout << "Time: " << atomicTime << " ms\n";

    return 0;
}
