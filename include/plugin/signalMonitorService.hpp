#ifndef PLUGIN_SIGNAL_MONITOR_SERVICE_HPP
#define PLUGIN_SIGNAL_MONITOR_SERVICE_HPP

#include <functional>

namespace reactorFramework
{

class signalMonitorService
{

public:
    using SignalHandler = std::function<void()>;

    signalMonitorService(const signalMonitorService&) = delete;
    signalMonitorService(signalMonitorService&&) = delete;
    signalMonitorService& operator=(const signalMonitorService&) = delete;
    signalMonitorService& operator=(signalMonitorService&&) = delete;

    virtual void add(int signal, const SignalHandler& sh) = 0;
    virtual void del(int signal) = 0 ;

protected:
    signalMonitorService() = default;

};

}
#endif

