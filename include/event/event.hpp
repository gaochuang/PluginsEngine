#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <stdint.h>
#include <sys/epoll.h>

namespace reactorFramework
{

using Callback = std::function<void()>;

class EventLoop;

class Event
{
public:
    //https://man7.org/linux/man-pages/man2/epoll_ctl.2.html

    const static int noneEventFlag;
    const static int readEventFlag;
    const static int writeEventFlag;
    const static int errorEventFlag;

    Event(EventLoop* loop, int fd);
    ~Event();

    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;
    Event(const Event&&) = delete;
    Event& operator=(const Event&&) = delete;

    int getFd() const;

    uint32_t getEvents() const;

    void handle(uint32_t revents);
    void removeFromLoop();

    void enableReading(bool isEnable);
    void enableWriting(bool isEnable) ;
    void enableErrorEvent(bool isEnable) ;
    void disableAll();
    bool isWriting();
    bool isReading();

    void setReadCallback(Callback cb);
    void setWriteCallback(Callback cb);
    void setErrorCallback(Callback cb);
    void setCloseCallback(Callback cb);


private:
    EventLoop* m_loop;
    int m_eventFd;
    uint32_t m_events;
  

    void update();

    Callback readCallback;
    Callback writeCallback;
    Callback errorCallback;
    Callback closeCallback;
   
};

}

#endif
