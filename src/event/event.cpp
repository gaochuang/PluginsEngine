#include "event/event.hpp"
#include "event/eventLoop.hpp"

#include <iostream>
#include <iomanip>
namespace commonApi
{

Event::Event(EventLoop* loop, int fd) : loop(loop), eventFd(fd), events(0)
{

}

Event::~Event()
{

}

void Event::enableReading(bool isEnable)
{
    if(isEnable)
        events |= readEventFlag;
    else
        events &= ~readEventFlag;
    update();
}

void Event::enableWriting(bool isEnable)
{
    if(isEnable)
        events |= writeEventFlag;
    else
        events &= ~writeEventFlag;
    update();
}

void Event::enableErrorEvent(bool isEnable)
{
    if(isEnable)
        events |= errorEventFlag;
    else
        events &= ~errorEventFlag;
    update();
}

void Event::disableAll()
{
    events = noneEventFlag;
    update();
}

bool Event::isWriting()
{
    return events & writeEventFlag;
}

bool Event::isReading()
{
    return events & readEventFlag;
}

int Event::getFd() const noexcept
{
   return eventFd;
}

uint32_t Event::getEvents() const noexcept
{
    return events;
}

void Event::setReadCallback(Callback cb)
{
    readCallback = std::move(cb);
}

void Event::setWriteCallback(Callback cb)
{
    writeCallback = std::move(cb);
}

void Event::setErrorCallback(Callback cb)
{
    errorCallback = std::move(cb);
}

void Event::setCloseCallback(Callback cb)
{
    closeCallback = std::move(cb);
}

void Event::update()
{
    loop->modifyEvent(eventFd);
}

void Event::removeFromLoop()
{
    loop->removeEvent(eventFd);
}

/*
如果callback中关闭fd，需要提前从epoll中移除 fd 
*/
void Event::handle(uint32_t events)
{
    //出现错误
    if (events & EPOLLERR)
    {
        if (errorCallback)
        {
            //如果callback中关闭fd，需要提前从epoll中移除 fd
            errorCallback();
        }
        return;
    }

    //EPOLLHUP 客户端主动关闭，会读到FIN包
    if ((events & EPOLLHUP) && !(events & EPOLLIN))
    {
        if (readCallback)
        {
            readCallback();
        }
    }

   // 对端socket 关闭，EPOLLRDHUP 是判断对端socket 是否关闭的首要标志位
    if (events & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (readCallback)
        {
            readCallback();
        }
    }

    if (events & EPOLLOUT)
    {
        if (writeCallback)
        {
            writeCallback();
        }
    }
}

}
