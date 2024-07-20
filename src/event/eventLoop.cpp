#include "eventLoop.hpp"
#include "thread.hpp"

#include <iostream>

namespace reactorFramework
{

const int EventLoop::PollTimeMs = 3000;

EventLoop::EventLoop() : eventCtrl(new EventCtrl(this)), 
                         threadId(Thread::getThreadId()),
                         timerQueue(new TimerQueue(this))
{

}

EventLoop::~EventLoop()
{

}


void EventLoop::addFunInLoop(Callback callback)
{
    std::unique_lock<std::mutex> lock(mutex);
    callbacks.push_back(callback);
}

void EventLoop::runAllFunctionInLoop()
{
    std::unique_lock<std::mutex> lock(mutex);
    for(auto &callback : callbacks)
    {
        callback();
    }

    if(!callbacks.empty())
    {
        callbacks.clear();
    }
}

bool EventLoop::inThisThread() const
{
    return threadId == Thread::getThreadId();
}

void EventLoop::addEvent(std::shared_ptr<Event> event)
{
    eventCtrl->addEvent(event);
}

void EventLoop::removeEvent(std::shared_ptr<Event> event)
{
    eventCtrl->deleteEvent(event);
}

void EventLoop::modifyEvent(std::shared_ptr<Event> event)
{
    eventCtrl->modifyEvent(event);
}

void EventLoop::modifyEvent(int fd)
{
    eventCtrl->modifyEvent(fd);
}

void EventLoop::removeEvent(int fd)
{
    eventCtrl->deleteEvent(fd);
}

void EventLoop::run()
{
    if(!inThisThread())
    {
        threadId = Thread::getThreadId();
        std::cout << "loop create and run in different thread" << threadId << std::endl;
    }

    while(true)
    {
        eventCtrl->waitAndRunHandler(PollTimeMs);
        runAllFunctionInLoop();
    }

}

void EventLoop::runInLoop(const Callback callback)
{
    if (inThisThread())
    {
        callback();
    }else
    {
        addFunInLoop(callback);
    }
    
}

//只执行一次的定时任务
void EventLoop::runOnceAfter(const Callback callback, uint32_t interval)
{
    timerQueue->runOnceAfter(callback, interval);
}

//循环执行定时任务
void EventLoop::runEveryInterval(const Callback callback, uint32_t interval)
{
    timerQueue->runEveryInterval(callback, interval);
}


}
