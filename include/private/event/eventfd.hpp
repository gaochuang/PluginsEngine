
#ifndef EVENT_FD_HPP
#define EVENT_FD_HPP

#include "eventLoop.hpp"
#include "event.hpp"
#include "plugin/CallbackQueueService.hpp"

#include "eventCallbackQueue.hpp"

#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <mutex>
#include <list>

#include <functional>

namespace commonApi
{

 using Callback = std::function<void()>;
class EventFD : public CallbackQueueService  
{
public:
        explicit EventFD(EventLoop* eventLoop);         
        ~EventFD() = default;

        EventFD(const EventFD&) = delete;
        EventFD& operator=(const EventFD&) = delete;
        EventFD(const EventFD&&) = delete;
        EventFD& operator=(const EventFD&&) = delete;

        void post(const Callback& callback) override;
        void post(Callback&& callback) override;
private:
        using Callbacks = std::list<Callback>;
        EventLoop* loop;
        int eventFd;
        std::shared_ptr<Event> event;
        std::mutex callbacksMutex;
        Callbacks callbacks;
    
        int createEventFd();

        template <typename CallbackType>
        void postImpl(CallbackType&& callback);

        template <typename CallbackType>
        void atomicPushBack(CallbackType&& callback);

        void handleEvents();
        Callbacks atomicPopAll();
        void executeCallbacks();
        void popAndExecuteFirstCallback(Callbacks& callbacks);
};

}
#endif
