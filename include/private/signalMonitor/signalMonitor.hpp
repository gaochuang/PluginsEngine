#ifndef SIGNAL_Monitor_HPP_
#define SIGNAL_Monitor_HPP_

#include "event/eventLoop.hpp"
#include <functional>
#include <unordered_map>
#include "plugin/SignalMonitorService.hpp"

namespace commonApi
{

class SignalMonitor : public SignalMonitorService
{
public:
    SignalMonitor(EventLoop* eventLoop);
    ~SignalMonitor();

    SignalMonitor(const SignalMonitor&) = delete;
    SignalMonitor(SignalMonitor&&) = delete;
    SignalMonitor& operator=(const SignalMonitor&) = delete;
    SignalMonitor& operator=(SignalMonitor&&) = delete;

    void add(int signal, const SignalHandler& sh) override;
    void del(int signal) override;

private:
    EventLoop* loop;
    int signalFd;
    std::shared_ptr<Event> event;
    sigset_t mask;
    std::unordered_map<int, SignalHandler> map;

    int createSignalFd();
    void block(int signal);
    void unblock(int signal);
    void addImpl(int signal, SignalHandler sh);
    void handleSignal();
};

}
#endif
