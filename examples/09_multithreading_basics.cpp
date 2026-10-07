#include <iostream>
#include <thread>

void worker()
{
    std::cout
        << "Worker thread ID: "
        << std::this_thread::get_id()
        << '\n';
}

int main()
{
    std::cout
        << "Main thread ID: "
        << std::this_thread::get_id()
        << '\n';

    std::thread workerThread(worker);

    std::cout
    << "Main continues on thread: "
    << std::this_thread::get_id()
    << '\n';

    workerThread.join();

    return 0;
}