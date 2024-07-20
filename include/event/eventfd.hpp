
#ifndef EVENT_FD_HPP
#define EVENT_FD_HPP

#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <mutex>

namespace reactorFramework
{

class EventFD 
{
public:
        explicit EventFD(int flags = EFD_CLOEXEC | EFD_NONBLOCK);         
        ~EventFD();
        void notify(uint64_t value = 1);
        uint64_t wait();

        EventFD(const EventFD&) = delete;
        EventFD& operator=(const EventFD&) = delete;
        EventFD(const EventFD&&) = delete;
        EventFD& operator=(const EventFD&&) = delete;
        int getEventFd() const;
private:
        int m_fd;
        mutable std::mutex mutex_;
};

}
#endif
