#ifndef EVENT_THREAD_POOL_HPP
#define EVENT_THREAD_POOL_HPP

#include "event/eventLoop.hpp"
#include "thread/eventThread.hpp"

namespace commonApi
{

class EventThreadPool
{
public: 
    EventThreadPool(EventLoop* loop);

    ~EventThreadPool();

    EventThreadPool(const EventThreadPool&) = delete;
    EventThreadPool& operator=(const EventThreadPool&) = delete;
    EventThreadPool(const EventThreadPool&&) = delete;
    EventThreadPool& operator=(const EventThreadPool&&) = delete;

    void init();
    void init(int num);
    bool isStarted() const;
    void setThreadNum(uint16_t num);

    EventLoop* getOneLoopFromPool();
    EventLoop* getOneLoopWithHash(int hash);
private: 
    EventLoop* mainLoop;
    bool started;
    uint16_t threadNum;
    uint16_t threadIndex;
    std::vector<std::shared_ptr<EventLoopThread> > threadPoll;
    std::vector<EventLoop*> eventLoopPoll;
};

}

#endif
