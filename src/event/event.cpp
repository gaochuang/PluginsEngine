#include "event.hpp"
#include "eventLoop.hpp"

namespace reactorFramework
{


const int Event::noneEventFlag = 0;
const int Event::readEventFlag = EPOLLIN | EPOLLPRI;
const int Event::writeEventFlag = EPOLLOUT;
const int Event::errorEventFlag = EPOLLERR;

Event::Event(EventLoop* loop, int fd) : m_loop(loop), m_eventFd(fd), m_events(0)
{

}

Event::~Event()
{

}

void Event::enableReading(bool isEnable)
{
    if(isEnable)
        m_events |= readEventFlag;
    else
        m_events &= ~readEventFlag;
    update();
}

void Event::enableWriting(bool isEnable)
{
    if(isEnable)
        m_events |= writeEventFlag;
    else
        m_events &= ~writeEventFlag;
    update();
}

void Event::enableErrorEvent(bool isEnable)
{
    if(isEnable)
        m_events |= errorEventFlag;
    else
        m_events &= ~errorEventFlag;
    update();
}

void Event::disableAll()
{
    m_events =  noneEventFlag;
    update();
}

bool Event::isWriting()
{
    return m_events & writeEventFlag;
}

bool Event::isReading()
{
    return m_events & readEventFlag;
}

int Event::getFd() const
{
   return m_eventFd;
}

uint32_t Event::getEvents() const
{
    return m_events;
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
    m_loop->modifyEvent(m_eventFd);
}

void Event::removeFromLoop()
{
    m_loop->removeEvent(m_eventFd);
}

void Event::handle(uint32_t events)
{
    if ((events & EPOLLHUP) && !(events & EPOLLIN))
    {
        if (readCallback)
        {
            readCallback();
        }
    }

    if (events & EPOLLERR)
    {
        if (errorCallback)
        {
            errorCallback();
        }
    }

   // EPOLLRDHUP Stream socket peer closed connection, or shut down writing half of connection
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

