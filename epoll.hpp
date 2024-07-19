#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include "event.hpp"

namespace reactorFramework
{

class Epoll 
{
public:
    Epoll();
    ~Epoll();

    Epoll(const Epoll&) = delete;
    Epoll& operator=(const Epoll&) = delete;
    Epoll(const Epoll&&) = delete;
    Epoll& operator=(const Epoll&&) = delete;

    bool addEvent(Event* ioEvent) noexcept;
    bool removeEvent(Event* ioEvent) noexcept;
    bool modifyEvent(Event* ioEvent) noexcept;
    bool removeEvent(int fd) noexcept;
    int waitEvent(struct epoll_event* eventList, int eventSize, int timeout) noexcept;

private:
   int epollCtrl(int operation, int fd, int events) noexcept;
   int m_epollFd;
};

}

#endif
