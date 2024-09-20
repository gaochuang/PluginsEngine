#ifndef COMMON_API_HPP
#define COMMON_API_HPP

#include "ComAPIEventLoop.hpp"

#include <chrono> // Add this line to include the chrono library

namespace commonApi
{

class ComAPI : public ComAPIEventLoop
{
public:
    using Callback = std::function<void()>;

    virtual ~ComAPI(){};
    virtual int getEpollFd() const = 0;
    virtual void postCallback(const Callback& callback) = 0;
    virtual void postCallback(Callback&& callback) = 0;
    virtual void waitAndHandleEvents() = 0;
    virtual bool waitAndHandleEvents(const std::chrono::steady_clock::duration& timeout) = 0;
    static std::shared_ptr<ComAPI> create();
protected:
    ComAPI() = default;
};

}

#endif
