#ifndef COMMON_API_PLUGIN_CALLBACK_QUEUE_HPP
#define COMMON_API_PLUGIN_CALLBACK_QUEUE_HPP

#include <functional>

namespace commonApi
{

class CallbackQueueService  
{
public:
    using Callback = std::function<void()>;

    CallbackQueueService(const CallbackQueueService&) = delete;
    CallbackQueueService(CallbackQueueService&&) = delete;
    CallbackQueueService& operator=(const CallbackQueueService&) = delete;
    CallbackQueueService& operator=(CallbackQueueService&&) = delete;

    virtual ~CallbackQueueService() {};

    virtual void post(const Callback& callback) = 0;
    virtual void post(Callback&& callback) = 0;

protected:
    CallbackQueueService() = default;
};

}
#endif
