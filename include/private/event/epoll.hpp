#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include "event.hpp"

namespace commonApi
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

    int getEpollFd() const noexcept;

    bool addEvent(Event* Event) noexcept;
    bool removeEvent(Event* Event) noexcept;
    bool modifyEvent(Event* Event) noexcept;
    bool removeEvent(int fd) noexcept;
    int waitEvent(struct epoll_event* eventList, int eventSize, int timeout) noexcept;

private:
   bool epollCtrl(int operation, int fd, int events) noexcept;
   int epollFd;
};

}

#endif
