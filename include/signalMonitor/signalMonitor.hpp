#ifndef SIGNAL_Monitor_HPP_
#define SIGNAL_Monitor_HPP_

#include "eventLoop.hpp"
#include <functional>
#include <unordered_map>

namespace reactorFramework
{

using SignalHandler = std::function<void()>;

class SignalMonitor
{
public:
    SignalMonitor(EventLoop* eventLoop);
    ~SignalMonitor();

    SignalMonitor(const SignalMonitor&) = delete;
    SignalMonitor(SignalMonitor&&) = delete;
    SignalMonitor& operator=(const SignalMonitor&) = delete;
    SignalMonitor& operator=(SignalMonitor&&) = delete;

    void add(int signal, const SignalHandler& sh);
    void del(int signal);

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
