
#include <cassert>
#include <iostream>

#include "spsc_queue.hpp"

int main()
{
    SPSCQueue<4> queue;

    // STEP 1: Fill the queue.
    for (std::uint64_t id = 1; id <= 4; ++id)
    {
        MarketDataEvent event{
            EventType::ADD,
            Side::BUY,
            id,
            20100,
            100
        };

        const bool success = queue.tryPush(event);

        assert(success);

        std::cout << "Push OrderId=" << id
                  << " success=" << std::boolalpha
                  << success << '\n';
    }

    // STEP 2: Verify that the queue is full.
    MarketDataEvent extraEvent{
        EventType::ADD,
        Side::BUY,
        5,
        20105,
        200
    };

    const bool pushedWhenFull = queue.tryPush(extraEvent);

    assert(!pushedWhenFull);

    std::cout << "Push when full: "
              << pushedWhenFull << '\n';

    // STEP 3: Consume the first event.
    MarketDataEvent received{};

    const bool firstPop = queue.tryPop(received);

    assert(firstPop);
    assert(received.order_id == 1);

    std::cout << "First pop OrderId="
              << received.order_id << '\n';

    // STEP 4: Reuse the freed slot.
    const bool pushedAfterPop = queue.tryPush(extraEvent);

    assert(pushedAfterPop);

    std::cout << "Push after pop: "
              << pushedAfterPop << '\n';

    // STEP 5: Verify FIFO ordering.
    for (std::uint64_t expectedId = 2;
         expectedId <= 5;
         ++expectedId)
    {
        const bool success = queue.tryPop(received);

        assert(success);
        assert(received.order_id == expectedId);

        std::cout << "Pop OrderId="
                  << received.order_id << '\n';
    }

    // STEP 6: Verify that the queue is empty.
    const bool poppedWhenEmpty = queue.tryPop(received);

    assert(!poppedWhenEmpty);

    std::cout << "Pop when empty: "
              << poppedWhenEmpty << '\n';

    std::cout << "All SPSC wraparound tests passed!\n";

    return 0;
}
