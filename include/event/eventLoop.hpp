/*
EventLoop类用来管理事件循环和回调函数的执行
*/

#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include "event.hpp"
#include "eventCtrl.hpp"
#include "timerQueue.hpp"
#include "eventCallbackQueue.hpp"

#include <memory>
#include <mutex>
#include <thread>

namespace reactorFramework
{

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;
    EventLoop(const EventLoop&&) = delete;
    EventLoop& operator=(const EventLoop&&) = delete;

    void addEvent(std::shared_ptr<Event> event);
    void removeEvent(std::shared_ptr<Event> event);
    void modifyEvent(std::shared_ptr<Event> event);
    void modifyEvent(int fd);
    void removeEvent(int fd);

    void run();
    void stop();

    void runInLoop(const Callback callback);
    void runOnceAfter(const Callback callback, uint32_t interval);
    void runEveryInterval(const Callback callback,uint32_t interval);

    void postCallback(const Callback& callback);
    void postCallback(Callback&& callback);
private:
    bool running;
    static const int PollTimeMs ;
    std::mutex mutex;
    std::shared_ptr<EventCtrl> eventCtrl;
    std::thread::id threadId;
    std::shared_ptr<TimerQueue> timerQueue;
    std::vector<Callback> callbacks;
    std::shared_ptr<eventCallbackQueue> callbackQueue;
    eventCallbackQueue& getEventCallbackQueue();
    void addFunInLoop(Callback func);
    void runAllFunctionInLoop();
    bool inThisThread() const;
};

}

#endif
