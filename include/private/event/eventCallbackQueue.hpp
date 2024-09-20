#ifndef EVENT_CALLBACK_QUEUE_HPP
#define EVENT_CALLBACK_QUEUE_HPP

#include <functional>

class eventCallbackQueue
{
public:
    using Callback = std::function<void()>;
    eventCallbackQueue(const eventCallbackQueue&) = delete;
    eventCallbackQueue(eventCallbackQueue&&) = delete;
    eventCallbackQueue& operator=(const eventCallbackQueue&) = delete;
    eventCallbackQueue& operator=(eventCallbackQueue&&) = delete;

    virtual ~eventCallbackQueue() {};

    virtual void post(const Callback& callback) = 0;
    virtual void post(Callback&& callback) = 0;
protected:
    eventCallbackQueue() = default;
};

#endif
