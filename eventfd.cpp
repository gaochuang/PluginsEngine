#include "eventfd.hpp"
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <stddef.h>
#include <sys/types.h>
#include <iostream>

namespace reactorFramework
{

EventFD::EventFD(int flags) : m_fd(-1)
{
    m_fd = ::eventfd(0, flags);
    if (m_fd == -1) 
    {
        throw std::runtime_error("Failed to create event fd");
    }
}

EventFD::~EventFD()
{
    ::close(m_fd);
}

void EventFD::notify(uint64_t value)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto ret = TEMP_FAILURE_RETRY(::write(m_fd, (void*)&value, sizeof(uint64_t)));
    if (ret != sizeof(uint64_t))
    {
        throw std::system_error(errno, std::system_category(), "write");
    }
}

uint64_t EventFD::wait()
{
    std::lock_guard<std::mutex> lock(mutex_);
    uint64_t value = 0;
    auto ret = TEMP_FAILURE_RETRY(::read(m_fd, (void*)&value, sizeof(uint64_t)));

    if ((-1 == ret) && (EAGAIN != errno))
    {
        throw std::system_error(errno, std::system_category(), "read");
    }else if ((-1 == ret) && (EAGAIN == errno))
    {
        std::cout << "read failed, reason: " << strerror(errno) << std::endl;
    }
    
    return value;
}

int EventFD::getEventFd() const
{
    return m_fd;
}

}
