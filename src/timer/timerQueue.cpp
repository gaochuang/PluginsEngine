#include "timer/timerQueue.hpp"
#include "event/eventLoop.hpp"
#include "timer/timer.hpp"

#include <sys/timerfd.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>

namespace commonApi
{

TimerQueue::TimerQueue(EventLoop* eventLoop):loop(eventLoop), timerFd(createTimerFd()),event(new Event(loop, timerFd))
{
    loop->addEvent(event);
    event->setReadCallback(std::bind(&TimerQueue::timerHandle, this));
    event->enableReading(true);
}

TimerQueue::~TimerQueue()
{
    ::close(timerFd);
}

int TimerQueue::createTimerFd()
{
    auto timerFd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(timerFd < 0)
    {
        std::cerr << "Failed to create timer fd" << std::endl;
    }

    return timerFd;
}

void TimerQueue::readTimerFd()
{
    uint64_t value;
    auto ret = TEMP_FAILURE_RETRY(::read(timerFd, (void*)&value, sizeof(uint64_t)));
    if(ret != sizeof(uint64_t))
    {
        std::cerr << "Failed to read timer fd" << std::endl;
    }
}

void TimerQueue::timerHandle()
{
    std::unique_lock<std::mutex> lock(mutex);
    readTimerFd();

    std::multimap<uint64_t,std::shared_ptr<Timer> >::iterator it;

    for(it = onceTimers.begin(); it != onceTimers.end(); )
    {
        if(it->first > Timer::getNowTimeMSecond())
        {
            break;;
        }

        it->second->timerHandle();
        it = onceTimers.erase(it);
    }

    for(it = recurringTimers.begin(); it != recurringTimers.end();)
    {
        //当前时间转成毫秒getNowTimeMSecond
        if (it->first > Timer::getNowTimeMSecond())
        {
            break;
        }

        it->second->timerHandle();
        std::shared_ptr<Timer> timer = it->second;
        timer->update();

        recurringTimers.insert(std::pair<uint64_t, std::shared_ptr<Timer> >(timer->getTimeOutMSecond(), timer));

        it = recurringTimers.erase(it);

    }

    //重置定时器
    resetTimer();
}

bool TimerQueue::needResetTimer(std::multimap<uint64_t, std::shared_ptr<Timer>>& timers, std::shared_ptr<Timer> timer)
{
    if(timers.empty())
    {
        return true;
    }

    //最早的计时器的超时时间早于当前计时器的超时时间，则不需要重置定时器
    auto it = timers.begin();
    if (it->first < timer->getTimeOutMSecond())
    {
        return false;
    }

    //已经有一个或多个计时器与当前计时器有相同的超时时间, 不需要重置定时器，因为这些计时器已经会在正确的时间被触发
    if(timers.count(timer->getTimeOutMSecond()) > 1)
    {
        return false;
    }
    return true;
}

void TimerQueue::resetTimer(std::shared_ptr<Timer> timer)
{
    struct itimerspec newValue = {};
    newValue.it_value  = timer->getTimeInterval(); //第一次设置后多久溢出
    auto ret = ::timerfd_settime(timerFd, 0, &newValue, nullptr);
    if (ret < 0)
    {
        std::cerr << "Failed to set timer fd" << " error: " << std::strerror(errno)<< std::endl;
    
    }
}

void TimerQueue::resetTimer()
{
    // 如果单次计时器队列为空，检查重复计时器队列是否为空
    if(onceTimers.empty())
    {
        //选择最早的计时器并调用 resetTimer 函数重置它
        if(!recurringTimers.empty())
        {
            auto it = recurringTimers.begin();
            resetTimer(it->second);
        }
    }else
    {
        //如果重复计时器队列为空，检查单次计时器队列是否为空
        if(recurringTimers.empty())
        {
            auto it = onceTimers.begin();
            resetTimer(it->second);
        }else
        {
            //如果两个队列都不为空，选择最早的计时器并调用 resetTimer 函数重置它
            auto it1 = recurringTimers.begin();
            auto it2 = onceTimers.begin();
            if(it1->second->getTimeOutMSecond() < it2->second->getTimeOutMSecond())
            {
                resetTimer(it1->second);
            }
            else
            {
                resetTimer(it2->second);
            }
        }
    }
}

void TimerQueue::addOnceTimer(const Callback& callback, uint32_t interval)
{
    std::shared_ptr<Timer> timer (new Timer(interval, callback));

    onceTimers.insert(std::pair<uint64_t,std::shared_ptr<Timer> >(timer->getTimeOutMSecond(), timer));

    if(needResetTimer(onceTimers, timer) && needResetTimer(recurringTimers, timer))
    {
        resetTimer(timer);
    }
}

void TimerQueue::runOnceAfter(const Callback& handler, int interval)
{
    std::unique_lock<std::mutex> lock(mutex);
    addOnceTimer(handler, interval);
}

void TimerQueue::addRecurringTimer(const Callback& handler, uint32_t interval)
{
    std::shared_ptr<Timer> timer(new Timer(interval, handler));
    recurringTimers.insert(std::pair<uint64_t, std::shared_ptr<Timer> >(timer->getTimeOutMSecond(), timer));
    if(needResetTimer(onceTimers, timer) && needResetTimer(recurringTimers, timer))
    {
        resetTimer(timer);
    }
}

void TimerQueue::runEveryInterval(const Callback& handler, int interval)
{
    std::unique_lock<std::mutex> lock(mutex);
    addRecurringTimer(handler, interval);
}

}
