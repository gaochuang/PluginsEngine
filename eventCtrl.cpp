#include "eventCtrl.hpp"

#include <iostream>
namespace reactorFramework
{

const int EventCtrl::activeEventLength = 20;

EventCtrl::EventCtrl(EventLoop* el) : eventLoop(el), activeEvents(activeEventLength)
{

}

EventCtrl::~EventCtrl()
{
    eventPool.clear();
}

void EventCtrl::addEvent(std::shared_ptr<Event> event)
{
    std::cout<<" event fd "<<event->getFd()<<std::endl;
    eventPool[event->getFd()] = event;
    epoll.addEvent(event.get());
}

void EventCtrl::deleteEvent(std::shared_ptr<Event> event)
{
    eventPool.erase(event->getFd());
    epoll.removeEvent(event.get());
}

void EventCtrl::modifyEvent(std::shared_ptr<Event> event)
{
    auto it = eventPool.find(event->getFd());
    if (it != eventPool.end())
    {
        epoll.modifyEvent(event.get());
    }
}

void EventCtrl::modifyEvent(int fd)
{
      auto it = eventPool.find(fd);
    if (it != eventPool.end())
    {
        std::shared_ptr<Event> eventS = it->second.lock();
        if (eventS)
        {
            epoll.modifyEvent(eventS.get());
        }
    }
}

void EventCtrl::deleteEvent(int fd)
{
    if (eventPool.find(fd) != eventPool.end())
    {
        epoll.removeEvent(fd);
        eventPool.erase(fd);
    }
}

void EventCtrl::waitAndRunHandler(int timeMs)
{
    auto num = epoll.waitEvent(&*activeEvents.begin(),activeEvents.size(),timeMs);
    if (num < 0)
    {
        return;
    }

    for(int i = 0; i < num; i++)
    {
        auto fd = activeEvents[i].data.fd;
        auto event = eventPool[fd].lock();
        if (event)
        {
            event->handle(activeEvents[i].events);
        }else
        {
            deleteEvent(fd);
        }
    }
}

}
