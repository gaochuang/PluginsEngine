/*
EventCtrl 责管理事件池和事件处理机制来添加、删除、修改事件，并在事件发生时调用相应的处理函数
*/
#ifndef EVENT_CTRL_HPP
#define EVENT_CTRL_HPP

#include "epoll.hpp"
#include "event.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace reactorFramework
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

    void addEvent(std::shared_ptr<Event> event);  
    void deleteEvent(std::shared_ptr<Event> event);
    void modifyEvent(std::shared_ptr<Event> event);
    void modifyEvent(int fd);
    void deleteEvent(int fd);
    void waitAndRunHandler(int timeMs);

private:
    static const int activeEventLength;
    std::unordered_map<int, std::weak_ptr<Event>> eventPool;

    Epoll epoll;
    EventLoop* eventLoop;
    std::vector<struct epoll_event> activeEvents;

};


}
#endif
