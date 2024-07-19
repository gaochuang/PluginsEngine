#include "epoll.hpp"

#include <iostream>
#include <unistd.h>
namespace reactorFramework
{

Epoll::Epoll() : m_epollFd(::epoll_create1(EPOLL_CLOEXEC))
{
    if (m_epollFd < 0)
    {
        std::cerr << "Failed to create epoll instance" << std::endl;
    }
}

Epoll::~Epoll()
{
    ::close(m_epollFd);
}

int Epoll::epollCtrl(int operation, int fd, int events) noexcept
{
    struct epoll_event event = {};

    event.events = events;
    event.data.fd = fd;

    return epoll_ctl(m_epollFd, operation, fd, &event);
}

bool Epoll::addEvent(Event* event) noexcept
{
    if (epollCtrl(EPOLL_CTL_ADD, event->getFd(), event->getEvents()) < 0)
    {
        std::cerr << "Failed to add event to epoll" << std::endl;
        return false;
    }
    return true;
}

bool Epoll::removeEvent(Event* event) noexcept
{
    if (epollCtrl(EPOLL_CTL_DEL, event->getFd(), event->getEvents()) < 0 )
    {
        std::cerr << "Failed to remove event from epoll" << std::endl;
        return false;
    }
    return true;    
}

bool Epoll::removeEvent(int fd) noexcept
{
    if (epollCtrl(EPOLL_CTL_DEL, fd, 0) < 0)
    {
        std::cerr << "Failed to remove event from epoll" << std::endl;
        return false;
    }
    return true;    

}

bool Epoll::modifyEvent(Event* event) noexcept
{
    if (epollCtrl(EPOLL_CTL_MOD, event->getFd(), event->getEvents()) < 0)
    {
        std::cerr << "Failed to modify event in epoll" << std::endl;
        return false;
    }
    return true;    
}

int Epoll::waitEvent(struct epoll_event* eventList, int eventSize, int timeout) noexcept
{
    return epoll_wait(m_epollFd, eventList, eventSize, timeout);

}

}

