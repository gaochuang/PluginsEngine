#ifndef PLUGIN_EVENT_SERVICE_HPP
#define PLUGIN_EVENT_SERVICE_HPP


#include <memory>
#include <functional>

namespace reactorFramework
{

class Event;

class FdMonitor
{
public:
    using FdEventHandler = std::function<void()>;
    virtual ~FdMonitor() = default;

    FdMonitor(const FdMonitor&) = delete;
    FdMonitor(FdMonitor&&) = delete;
    FdMonitor& operator=(const FdMonitor&) = delete;
    FdMonitor& operator=(FdMonitor&&) = delete;

    static const unsigned int EVENT_IN;
    static const unsigned int EVENT_OUT;
    static const unsigned int EVENT_ERR;
    static const unsigned int EVENT_HUP;

    virtual void addFd(int fd, unsigned int events, FdEventHandler& handler) = 0;
    virtual void removeFd(int fd) = 0;
    virtual void modifyFd(int fd, unsigned int events) = 0;
protected:
    FdMonitor() = default;
};

}

#endif
