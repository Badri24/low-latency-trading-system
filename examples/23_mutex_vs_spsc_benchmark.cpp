
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <thread>

#include "market_data_event.hpp"
#include "spsc_queue.hpp"
#include "thread_safe_queue.hpp"

// Number of events transferred during each benchmark.
constexpr std::uint64_t EVENT_COUNT = 1'000'000;

// Physical capacity of the bounded SPSC queue.
constexpr std::size_t SPSC_CAPACITY = 1024;

// Store both performance and correctness information.
struct BenchmarkResult
{
    double elapsedMs = 0.0;
    std::uint64_t receivedCount = 0;
    bool valid = true;
    std::uint64_t retries = 0;
};

// Create an event with deterministic data.
// The consumer can verify it without shared state.
MarketDataEvent makeEvent(std::uint64_t id)
{
    return MarketDataEvent{
        EventType::ADD,
        Side::BUY,
        id,
        20100 + static_cast<std::int64_t>(id),
        100
    };
}

// Check that the consumer received the expected event.
bool validateEvent(const MarketDataEvent& event,
                   std::uint64_t expectedId)
{
    return event.order_id == expectedId
        && event.price ==
            20100 + static_cast<std::int64_t>(expectedId)
        && event.quantity == 100
        && event.type == EventType::ADD
        && event.side == Side::BUY;
}

// -----------------------------------------------------
// Benchmark 1: Mutex + condition_variable
// -----------------------------------------------------
BenchmarkResult benchmarkMutexQueue()
{
    ThreadSafeQueue queue;
    BenchmarkResult result;

    const auto start = std::chrono::steady_clock::now();

    // Consumer reads until the queue is closed and empty.
    std::thread consumer([&]()
    {
        MarketDataEvent event{};
        std::uint64_t expectedId = 1;

        while (queue.waitAndPop(event))
        {
            if (!validateEvent(event, expectedId))
            {
                result.valid = false;
            }

            ++expectedId;
            ++result.receivedCount;
        }
    });

    // Producer generates exactly EVENT_COUNT events.
    std::thread producer([&]()
    {
        for (std::uint64_t id = 1;
             id <= EVENT_COUNT;
             ++id)
        {
            queue.push(makeEvent(id));
        }

        // Wake the consumer and allow it to finish
        // once all queued events have been consumed.
        queue.close();
    });

    producer.join();
    consumer.join();

    const auto end = std::chrono::steady_clock::now();

    result.elapsedMs =
        std::chrono::duration<double, std::milli>(
            end - start
        ).count();

    result.valid =
        result.valid &&
        result.receivedCount == EVENT_COUNT;

    return result;
}

// -----------------------------------------------------
// Benchmark 2: SPSC lock-free ring buffer
// -----------------------------------------------------
BenchmarkResult benchmarkSPSCQueue()
{
    SPSCQueue<SPSC_CAPACITY> queue;
    BenchmarkResult result;

    const auto start = std::chrono::steady_clock::now();

    // Consumer expects exactly EVENT_COUNT events.
    std::thread consumer([&]()
    {
        MarketDataEvent event{};

        for (std::uint64_t expectedId = 1;
             expectedId <= EVENT_COUNT;
             ++expectedId)
        {
            // The queue may temporarily be empty.
            while (!queue.tryPop(event))
            {
                ++result.retries;
                std::this_thread::yield();
            }

            if (!validateEvent(event, expectedId))
            {
                result.valid = false;
            }

            ++result.receivedCount;
        }
    });

    std::thread producer([&]()
    {
        for (std::uint64_t id = 1;
             id <= EVENT_COUNT;
             ++id)
        {
            const MarketDataEvent event = makeEvent(id);

            // The queue is bounded: retry when full.
            while (!queue.tryPush(event))
            {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();

    const auto end = std::chrono::steady_clock::now();

    result.elapsedMs =
        std::chrono::duration<double, std::milli>(
            end - start
        ).count();

    result.valid =
        result.valid &&
        result.receivedCount == EVENT_COUNT;

    return result;
}

// -----------------------------------------------------
// Print benchmark results
// -----------------------------------------------------
void printResult(const char* name,
                 const BenchmarkResult& result)
{
    // Convert elapsed milliseconds into seconds.
    const double elapsedSeconds =
        result.elapsedMs / 1000.0;

    // Events transferred per second, in millions.
    const double throughput =
        static_cast<double>(result.receivedCount)
        / elapsedSeconds
        / 1'000'000.0;

    std::cout << name
              << " | Time: " << result.elapsedMs << " ms"
              << " | Throughput: " << throughput << " M events/s"
              << " | Valid: " << std::boolalpha << result.valid
              << '\n';
}

int main()
{
    std::cout << std::fixed << std::setprecision(3);

    constexpr int RUNS = 5;

    // Keep track of average execution times.
    double mutexTotalMs = 0.0;
    double spscTotalMs = 0.0;

    for (int run = 1; run <= RUNS; ++run)
    {
        std::cout << "\n=== RUN " << run << " ===\n";

        BenchmarkResult mutexResult;
        BenchmarkResult spscResult;

        // Alternate execution order to reduce
        // systematic first/second benchmark bias.
        if (run % 2 == 1)
        {
            mutexResult = benchmarkMutexQueue();
            spscResult = benchmarkSPSCQueue();
        }
        else
        {
            spscResult = benchmarkSPSCQueue();
            mutexResult = benchmarkMutexQueue();
        }

        printResult("Mutex", mutexResult);
        printResult("SPSC ", spscResult);

        if (!mutexResult.valid || !spscResult.valid)
        {
            std::cerr << "Benchmark validation failed!\n";
            return 1;
        }

        mutexTotalMs += mutexResult.elapsedMs;
        spscTotalMs += spscResult.elapsedMs;
    }

    const double mutexAverage = mutexTotalMs / RUNS;
    const double spscAverage = spscTotalMs / RUNS;

    std::cout << "\n===== AVERAGE RESULTS =====\n";

    std::cout << "Mutex average: "
              << mutexAverage << " ms\n";

    std::cout << "SPSC average: "
              << spscAverage << " ms\n";

    std::cout << "Mutex / SPSC time ratio: "
              << mutexAverage / spscAverage
              << "x\n";

    return 0;
}
