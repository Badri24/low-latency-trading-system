#include <iostream>
#include <mutex>
#include <thread>

int counter = 0;

std::mutex counterMutex;
/*
void incrementCounter()
{
    for (int i = 0; i < 1'000'000; ++i)
    {
        counterMutex.lock();

        counter++;

        counterMutex.unlock();
    }
}*/

void incrementCounter()
{
    for (int i = 0; i < 1000000; ++i)
    {
        std::lock_guard<std::mutex> lock(counterMutex);

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