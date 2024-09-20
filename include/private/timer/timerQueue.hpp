#ifndef TIMER_QUEUE_HPP
#define TIMER_QUEUE_HPP

#include "event/event.hpp"
#include "timer.hpp"
#include "plugin/TimerService.hpp"

#include <memory>
#include <map>
#include <mutex>

namespace commonApi
{

class EventLoop;

class TimerQueue : public TimerService
{
public:
    TimerQueue(EventLoop* eventLoop);
    ~TimerQueue();

    TimerQueue(const TimerQueue&) = delete;
    TimerQueue& operator=(const TimerQueue&) = delete;
    TimerQueue(const TimerQueue&&) = delete;
    TimerQueue& operator=(const TimerQueue&&) = delete;
 
    void addOnceTimer(const Callback& callback, uint32_t interval);
    void runOnceAfter(const Callback& handler, int interval);
    void addRecurringTimer(const Callback& handler, uint32_t interval);
    void runEveryInterval(const Callback& handler, int interval);

private:
   EventLoop* loop;
   int timerFd;
   std::shared_ptr<Event> event;
   std::mutex mutex;

    std::multimap<uint64_t, std::shared_ptr<Timer>> recurringTimers; // 重复定时器
    std::multimap<uint64_t, std::shared_ptr<Timer>> onceTimers; // 一次性定时器

    int  createTimerFd();
    void timerHandle();
    bool needResetTimer(std::multimap<uint64_t, std::shared_ptr<Timer>>& timers, std::shared_ptr<Timer> timer);
    void resetTimer(std::shared_ptr<Timer> timer);
    void resetTimer();
    void readTimerFd();
};


}

#endif
