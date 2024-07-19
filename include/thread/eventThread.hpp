#include "thread.hpp"
#include "eventLoop.hpp"

namespace reactorFramework
{

class EventLoopThread : public Thread
{
public:
    EventLoopThread();

    EventLoopThread(const EventLoopThread&) = delete;
    EventLoopThread& operator=(const EventLoopThread&) = delete;
    EventLoopThread(const EventLoopThread&&) = delete;
    EventLoopThread& operator=(const EventLoopThread&&) = delete;

    virtual void run();
    EventLoop *getEventLoopInThread();
private:
    EventLoop* loop;
    std::mutex mutex;
    std::condition_variable condition;
};

}
