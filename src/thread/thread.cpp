#include "thread.hpp"

namespace reactorFramework
{

Thread::Thread():started(false),thread(new std::thread(&Thread::beginRun, this))
{
}

Thread::~Thread()
{

}

void Thread::start()
{
    std::unique_lock<std::mutex> lock(mutex);
    started = true;
    condition.notify_one();
}

void Thread::beginRun()
{
    std::unique_lock<std::mutex> lock(mutex);
    while (!started)
    {
        condition.wait(lock);
    }
    run();
}

bool Thread::isStarted() const
{
    return started;
}

void Thread::sleepSeconds(int seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

std::thread::id Thread::getThreadId()
{
    return std::this_thread::get_id();
}

}