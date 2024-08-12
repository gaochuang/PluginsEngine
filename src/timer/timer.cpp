#include "timer.hpp"
#include <sstream>

#include <iostream>
namespace reactorFramework
{

Timer::Timer(uint32_t interval, const TimerCallBack& callback): intervalMs(interval), callback(callback)
{
    update();
}

struct timespec Timer::getTimeout()
{
    struct timespec timeout = now;


    // 加上时间间隔
    timeout.tv_nsec += (intervalMs % 1000) * 1000000; // 毫秒部分转为纳秒
    timeout.tv_sec += intervalMs / 1000;  // 秒部分

    // 确保纳秒部分不超过1000000000（即一秒）
    if (timeout.tv_nsec >= 1000000000)
    {
        timeout.tv_sec += timeout.tv_nsec / 1000000000;
        timeout.tv_nsec %= 1000000000;
    }

    return timeout;
}

uint64_t Timer::getTimeOutMSecond()
{
    struct timespec timeout = getTimeout();
    
    auto mSecond = timeout.tv_sec * 1000 + timeout.tv_nsec / 1000000;

    return mSecond;
}

uint64_t Timer::getNowTimeMSecond()
{
    struct timespec nowDate = {};
    clock_gettime(CLOCK_MONOTONIC, &nowDate);

    auto ms = nowDate.tv_sec * 1000 + nowDate.tv_nsec / 1000000;

    return ms;
}

std::string Timer::getNowTimeDate()
{
    time_t timep;
    time(&timep);

    auto p = ::localtime(&timep);

    std::stringstream stream;
    stream << (1900 + p->tm_year) << "-" << (1 + p->tm_mon) << "-" << p->tm_mday << " " << p->tm_hour << ":" << p->tm_min << ":" << p->tm_sec;
    return stream.str();
}

struct timespec Timer::getTimeInterval() 
{
    auto interval = getTimeOutMSecond() - getNowTimeMSecond();
    if (interval <= 15)
    {
        //避免高频调用
        interval = 15;
        std::cout << " use default interval: " << interval << " ms" << std::endl;
    }

    struct timespec timerInterval = {};
    timerInterval.tv_sec = interval / 1000;
    timerInterval.tv_nsec = (interval % 1000) * 1000000;

    return timerInterval;
}

void Timer::update()
{
    ::clock_gettime(CLOCK_MONOTONIC, &now);
}

void Timer::setHandle(const TimerCallBack& cb)
{
    callback = cb;
}

void Timer::timerHandle()
{
    callback();
}

}
