
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

constexpr int ITERATIONS = 1'000'000;

std::atomic<std::uint64_t> seqCounter{0};
std::atomic<std::uint64_t> relaxedCounter{0};

void incrementSeqCst()
{
    for (int i = 0; i < ITERATIONS; ++i)
    {
        seqCounter.fetch_add(
            1,
            std::memory_order_seq_cst
        );
    }
}

void incrementRelaxed()
{
    for (int i = 0; i < ITERATIONS; ++i)
    {
        relaxedCounter.fetch_add(
            1,
            std::memory_order_relaxed
        );
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
    const double seqTime = benchmark(incrementSeqCst);
    const double relaxedTime = benchmark(incrementRelaxed);

    std::cout << "Expected: " << 2 * ITERATIONS << '\n';

    std::cout << "\nSequential consistency:\n";
    std::cout << "Counter: " << seqCounter.load() << '\n';
    std::cout << "Time: " << seqTime << " ms\n";

    std::cout << "\nRelaxed memory ordering:\n";
    std::cout << "Counter: " << relaxedCounter.load() << '\n';
    std::cout << "Time: " << relaxedTime << " ms\n";

    return 0;
}
