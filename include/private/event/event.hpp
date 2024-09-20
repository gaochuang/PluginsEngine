#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <stdint.h>
#include <sys/epoll.h>

namespace commonApi
{

using Callback = std::function<void()>; //函数包装器，函数对象

class EventLoop;

class Event
{
public:
    //https://man7.org/linux/man-pages/man2/epoll_ctl.2.html

    static constexpr int noneEventFlag = 0;
    static constexpr int readEventFlag =  EPOLLIN | EPOLLPRI;
    static constexpr int writeEventFlag = EPOLLOUT;
    static constexpr int errorEventFlag = EPOLLERR;

    using EventHandler = std::function<void(unsigned int events)>;

    Event(EventLoop* loop, int fd);
    ~Event();

    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;
    Event(const Event&&) = delete;
    Event& operator=(const Event&&) = delete;

    int getFd() const noexcept;

    uint32_t getEvents() const noexcept;

    void handle(uint32_t revents);
    void removeFromLoop();

    void enableReading(bool isEnable);
    void enableWriting(bool isEnable) ;
    void enableErrorEvent(bool isEnable);
    void disableAll();
    bool isWriting();
    bool isReading();

    void setReadCallback(Callback cb);
    void setWriteCallback(Callback cb);
    void setErrorCallback(Callback cb);
    void setCloseCallback(Callback cb);

private:
    EventLoop* loop;
    int eventFd;
    uint32_t events;

    Callback readCallback;
    Callback writeCallback;
    Callback errorCallback;
    Callback closeCallback;

    void update();
};

}

#endif
