/*
EventCtrl 维护事件注册表、事件分发功能
*/
#ifndef EVENT_CTRL_HPP
#define EVENT_CTRL_HPP

#include "epoll.hpp"
#include "event.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace commonApi
{

class EventLoop;

class EventCtrl
{
public:
    EventCtrl(EventLoop* el);
    ~EventCtrl();

    EventCtrl(const EventCtrl&) = delete;
    EventCtrl& operator=(const EventCtrl&) = delete;
    EventCtrl(const EventCtrl&&) = delete;
    EventCtrl& operator=(const EventCtrl&&) = delete;

    int getEpollFd() const noexcept;
    void addEvent(std::shared_ptr<Event> event);  
    void deleteEvent(std::shared_ptr<Event> event);
    void modifyEvent(std::shared_ptr<Event> event);
    void modifyEvent(int fd);
    void deleteEvent(int fd);
    void waitAndRunHandler(int timeMs);

private:
    static const int activeEventLength;
    //事件注册表
    std::unordered_map<int, std::weak_ptr<Event>> eventPool;

    Epoll epoll;
    EventLoop* eventLoop;
    //从epoll读取到的事件
    std::vector<struct epoll_event> activeEvents;

};


}
#endif
