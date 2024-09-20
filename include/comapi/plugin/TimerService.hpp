#ifndef  COMMON_API_PLUGIN_TIMER_SERVICE_HPP
#define  COMMON_API_PLUGIN_TIMER_SERVICE_HPP

#include <functional>
#include <cstdint>

namespace commonApi
{

class TimerService
{
public:
    using Callback = std::function<void()>;

    TimerService(const TimerService&) = delete;
    TimerService(TimerService&&) = delete;
    TimerService& operator=(const TimerService&) = delete;
    TimerService& operator=(TimerService&&) = delete;

    virtual void addOnceTimer(const Callback& callback, uint32_t interval) = 0;
    virtual void runOnceAfter(const Callback& handler, int interval) = 0;
    virtual void addRecurringTimer(const Callback& handler, uint32_t interval) = 0;
    virtual void runEveryInterval(const Callback& handler, int interval) = 0;
protected:
    TimerService() = default;

};

}

#endif
