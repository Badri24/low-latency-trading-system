#include <iostream>
#include <thread>

int counter = 0;

void incrementCounter()
{
    for (int i = 0; i < 1'000'000; ++i)
    {
        counter++;
    }
}

int main()
{
    std::thread thread1(incrementCounter);
    std::thread thread2(incrementCounter);

    thread1.join();
    thread2.join();

    std::cout
        << "Expected counter: 2000000\n"
        << "Actual counter:   " << counter
        << '\n';

    return 0;
}