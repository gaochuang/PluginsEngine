
#include "commonapi.h"

#include "ComAPI.hpp"
#include "ComAPIEventLoop.hpp"
#include "GetSharedPtr.hpp"
#include <optional>
#include <stdlib.h>

using namespace commonApi;

namespace
{

class Handle
{
public:
   explicit Handle(std::shared_ptr<ComAPI>&& com);

   ~Handle() noexcept;

   [[nodiscard]] int fd() const noexcept;

   void handleEvents() noexcept;

    std::shared_ptr<ComAPI> get();

   static Handle* get(comApi_handle handle);


   Handle(const Handle&) = delete;
   Handle(Handle&&) = delete;
   Handle operator=(const Handle&) = delete;
   Handle operator=(Handle&&) = delete;
private:
    static const int MAGIC = 0xFFFFEEEE;
    int magic;
    std::shared_ptr<ComAPI> com;
};

Handle::Handle(std::shared_ptr<ComAPI>&& com):
    magic(MAGIC),
    com(std::move(com))
{
}

Handle::~Handle()
{
    magic = 0;
}

int Handle::fd() const noexcept
{
    return com->getEpollFd();
}

void Handle::handleEvents() noexcept
{
    com->waitAndHandleEvents(std::chrono::milliseconds(0));
}

std::shared_ptr<ComAPI> Handle::get()
{
    return com;
}

Handle* Handle::get(comApi_handle handle)
{
    auto ret = static_cast<Handle*> (handle);
    if(nullptr == ret || ret->magic != MAGIC)
    {
        abort();
    }

    return ret;
}

}

comApi_handle comApi_create()
{
    try
    {
        return new Handle(ComAPI::create());
    }
    catch(...)
    {
        return nullptr;
    }

}

void comApi_destroy(comApi_handle handle)
{
    if(nullptr != handle)
    {
        delete Handle::get(handle);
    }
}

int comApi_fd(comApi_handle handle)
{
    return Handle::get(handle)->fd();
}

void comApi_handle_event(comApi_handle handle)
{
    Handle::get(handle)->handleEvents();
}

std::shared_ptr<ComAPI> getSharedPtr(comApi_handle handle)
{
    return Handle::get(handle)->get();
}
