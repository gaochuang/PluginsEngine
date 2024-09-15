#ifndef COMMON_API_EVENTLOOP_HPP
#define COMMON_API_EVENTLOOP_HPP

#include <functional>
#include <memory>

namespace reactorFramework
{

class ComAPIEventLoop
{
public:
    using EventHandler = std::function<void(unsigned int events)>;
    using Add = std::function<void(int fd, unsigned int events, const EventHandler& eventHandler)>;
    using Modify = std::function<void(int fd, unsigned int newEvents)>;
    using Del = std::function<void(int fd)>;

    virtual  ~ComAPIEventLoop() {};

    static const unsigned int EVENT_IN;
    static const unsigned int EVENT_OUT;
    static const unsigned int EVENT_ERR;
    static const unsigned int EVENT_HUP;

    struct Callbacks
    {
        Add add;
        Modify modify;
        Del del;
    };

    ComAPIEventLoop(const ComAPIEventLoop&) = delete;
    ComAPIEventLoop& operator=(const ComAPIEventLoop&) = delete;
    ComAPIEventLoop(const ComAPIEventLoop&&) = delete;
    ComAPIEventLoop& operator=(const ComAPIEventLoop&&) = delete;

    static std::shared_ptr<ComAPIEventLoop> create(const Callbacks& callbacks);
    
protected:
    ComAPIEventLoop() = default;
};

}
#endif
