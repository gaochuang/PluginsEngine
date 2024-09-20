#include "event/eventfd.hpp"

#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <stddef.h>
#include <sys/types.h>
#include <iostream>
#include <utility>

namespace commonApi
{

void execute(const Callback& callback) noexcept
{
    callback();
}

EventFD::EventFD(EventLoop* eventLoop):loop(eventLoop), eventFd(createEventFd()), event(std::make_shared<Event>(loop, eventFd)) 
{
    loop->addEvent(event);
    event->enableReading(true);
    event->setReadCallback(std::bind(&EventFD::handleEvents, this));
}

int EventFD::createEventFd()
{
    const int ret(::eventfd(0U,  EFD_NONBLOCK | EFD_CLOEXEC));
    if (ret == -1)
    {
        std::cout << "event fd error" << ::strerror(errno)<<std::endl;
    }
    
    return ret;
}


void EventFD::post(const Callback& callback)
{
    postImpl(callback);
}

void EventFD::post(Callback&& callback)
{
    postImpl(std::move(callback));
}

template <typename CallBackType>
void EventFD::postImpl(CallBackType&& callback)
{
    if(!callback)
    {
        std::cerr << "callback is empty" << std::endl;
        return;
    }
    atomicPushBack(std::forward<CallBackType>(callback));
    static const uint64_t value(1U);
    ::write(eventFd, &value, sizeof(value));
}

template <typename CallbackType>
void EventFD::atomicPushBack(CallbackType&& callback)
{
    std::unique_lock<std::mutex> lock(callbacksMutex);
    callbacks.push_back(std::forward<CallbackType>(callback));
}

EventFD::Callbacks EventFD::atomicPopAll()
{
    std::unique_lock<std::mutex> lock(callbacksMutex);
    Callbacks extractedCallbacks;
    std::swap(callbacks, extractedCallbacks);
    return extractedCallbacks;
}

void EventFD::handleEvents()
{
    uint64_t value;
    ::read(eventFd, &value, sizeof(value));
    executeCallbacks();
}

void EventFD::executeCallbacks()
{
    Callbacks callbacks(atomicPopAll());
    while (!callbacks.empty())
    {
        popAndExecuteFirstCallback(callbacks);
    }   
}

void EventFD::popAndExecuteFirstCallback(Callbacks& callbacks)
{
    const auto callback(std::move(callbacks.front()));
    callbacks.pop_front();
    execute(callback);
}

}
