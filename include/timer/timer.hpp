#ifndef TIMER_HPP
#define TIMER_HPP

#include <stdint.h>
#include <sys/time.h>
#include <functional>
#include <memory>

namespace reactorFramework
{

using TimerCallBack = std::function<void()>;

class Timer
{
public:
    Timer(uint32_t interval, const TimerCallBack& callback);

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(const Timer&&) = delete;
    Timer& operator=(const Timer&&) = delete;

    struct timeval getTimeout();
    uint64_t getTimeOutMSecond();
    struct timespec getTimeInterval();
    static uint64_t getNowTimeMSecond();
    static std::string getNowTimeDate();
   
    void update();
    void setHandle(const TimerCallBack&);
    void timerHandle();


private:
   uint32_t intervalMs;
   struct timeval now;
   TimerCallBack callback;
};

}
#endif
