#include "thread/eventThread.hpp"

namespace commonApi
{

EventLoopThread::EventLoopThread(): loop(nullptr)
{
}

EventLoop* EventLoopThread::getEventLoopInThread()
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        if(nullptr == loop)
        {
            condition.wait(lock);
        }
    }

    return loop;
}

void EventLoopThread::run()
{
    EventLoop loop ;
    {
        std::unique_lock<std::mutex> lock(mutex);
        this->loop = &loop;
        condition.notify_one();
    }

    loop.run();
}

}
