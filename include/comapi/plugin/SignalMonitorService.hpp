#ifndef COMMON_API_PLUGIN_PLUGIN_SIGNAL_MONITOR_SERVICE_HPP
#define COMMON_API_PLUGIN_PLUGIN_SIGNAL_MONITOR_SERVICE_HPP

#include <functional>

namespace commonApi
{

class SignalMonitorService
{

public:
    using SignalHandler = std::function<void()>;

    SignalMonitorService(const SignalMonitorService&) = delete;
    SignalMonitorService(SignalMonitorService&&) = delete;
    SignalMonitorService& operator=(const SignalMonitorService&) = delete;
    SignalMonitorService& operator=(SignalMonitorService&&) = delete;

    virtual void add(int signal, const SignalHandler& sh) = 0;
    virtual void del(int signal) = 0 ;

protected:
    SignalMonitorService() = default;

};

}
#endif

