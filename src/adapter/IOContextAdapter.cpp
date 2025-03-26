#include "IOContextAdapter.hpp"

#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/asio/post.hpp>
#include <unordered_map>
#include <poll.h>

using namespace commonApi;

namespace
{
    constexpr unsigned int ERROR_EVENT(1U << 24);
}

class IOContextAdapter::adapterImpl
{
public:
    explicit adapterImpl(boost::asio::io_context& ioc);

    ~adapterImpl();

    adapterImpl(const adapterImpl& ) = delete;
    adapterImpl(adapterImpl&& ) = delete;
    adapterImpl& operator=(const adapterImpl& ) = delete;
    adapterImpl& operator=(adapterImpl&& ) = delete;

    [[nodiscard]] const std::shared_ptr<ComAPIEventLoop>& getComAPIEventLoop() const noexcept { return comAPIEventLoop; }
private:
    struct StreamDesc
    {
      StreamDesc(boost::asio::io_context& ioc, int fd, unsigned int events, 
                 const ComAPIEventLoop::EventHandler& handler):streamDescriptor(ioc, fd),
                                                               events(events),
                                                               eventHandler(handler),
                                                               waitedEvents(0U),
                                                               collectedEvents(0U)

      {
      }
      boost::asio::posix::stream_descriptor streamDescriptor;
      unsigned int events;
      ComAPIEventLoop::EventHandler eventHandler;
      bool eventHandlerPosted;
      unsigned int waitedEvents;
      unsigned int collectedEvents;
    };

    void add(int fd, unsigned int events, const ComAPIEventLoop::EventHandler& handler);
    void modify(int fd, unsigned int evnets);
    void del(int fd);
    void waitAsync(const std::shared_ptr<StreamDesc>& descr);
    void handleAndWait(const boost::system::error_code& err, const std::weak_ptr<StreamDesc>& weakDescr, unsigned int events);
    void handleCollectEvents(const std::weak_ptr<StreamDesc>& weakDescr);

    std::shared_ptr<bool> isAlive;
    boost::asio::io_context& ioc;
    std::unordered_map<int, std::shared_ptr<StreamDesc>> map;
    std::shared_ptr<ComAPIEventLoop> comAPIEventLoop;
};

IOContextAdapter::adapterImpl::adapterImpl(boost::asio::io_context& ioc):isAlive(std::make_shared<bool>(true)),ioc(ioc)
{
    const std::weak_ptr<bool> weakIsAlive(isAlive);

    ComAPIEventLoop::create({ [this, weakIsAlive](int fd, unsigned int events, const ComAPIEventLoop::EventHandler& handler)
                              {
                                if(weakIsAlive.lock())
                                {
                                    add(fd, events, handler);
                                }
                               },
                               [this, weakIsAlive](int fd, unsigned int events)
                               {
                                   if(weakIsAlive.lock())
                                   {
                                        modify(fd, events);
                                   }
                               },

                               [this, weakIsAlive](int fd)
                               {
                                    del(fd);
                               }});

}

IOContextAdapter::adapterImpl::~adapterImpl()
{
    comAPIEventLoop.reset();
    for(const auto& i : map)
    {
        i.second->streamDescriptor.release();
    }
}

void IOContextAdapter::adapterImpl::add(int fd, unsigned int events, const ComAPIEventLoop::EventHandler& handler)
{
    auto i = map.insert(std::make_pair(fd, std::make_shared<StreamDesc>(ioc, fd, events, handler)));
    if(i.second)
    {
        throw "add events failed";
    }

    waitAsync(i.first->second);
}

void IOContextAdapter::adapterImpl::modify(int fd, unsigned int evnets)
{
    auto i = map.find(fd);
    if(i == map.end())
    {
        throw "no this fd";
    }

    i->second->events = evnets;
    waitAsync(i->second);
}

void IOContextAdapter::adapterImpl::del(int fd)
{
    auto i = map.find(fd);
    if(i == map.end())
    {
        throw "no this fd";
    }

    i->second->streamDescriptor.release();
    map.erase(i);
}

void IOContextAdapter::adapterImpl::waitAsync(const std::shared_ptr<StreamDesc>& descr)
{
    const std::weak_ptr<StreamDesc> weakDescrPtr = descr;

    if((descr->events & ComAPIEventLoop::EVENT_IN) && ((descr->waitedEvents & ComAPIEventLoop::EVENT_IN) == 0U))
    {
        //注册等待可读事件
        descr->streamDescriptor.async_wait(boost::asio::posix::stream_descriptor::wait_read,
                                          std::bind(&adapterImpl::handleAndWait, this, std::placeholders::_1, weakDescrPtr, 
                                          ComAPIEventLoop::EVENT_IN));
        descr->waitedEvents |= ComAPIEventLoop::EVENT_IN;
    }
   
    //注册等待可读事件
    if((descr->events & ComAPIEventLoop::EVENT_OUT) && ((descr->waitedEvents &ComAPIEventLoop::EVENT_OUT) == 0U))
    {
        descr->streamDescriptor.async_wait(boost::asio::posix::stream_descriptor::wait_write,
                                           std::bind(&adapterImpl::handleAndWait, this, std::placeholders::_1, weakDescrPtr,
                                            ComAPIEventLoop::EVENT_OUT));
        descr->waitedEvents |= ComAPIEventLoop::EVENT_OUT;
    }
    
    //错误事件
    if((descr->waitedEvents & ERROR_EVENT) == 0U)
    {
        descr->streamDescriptor.async_wait(boost::asio::posix::stream_descriptor::wait_error,
                                           std::bind(&adapterImpl::handleAndWait, this, std::placeholders::_1, weakDescrPtr, ERROR_EVENT));
        descr->waitedEvents |= ERROR_EVENT;
    }

}

void IOContextAdapter::adapterImpl::handleAndWait(const boost::system::error_code& err, const std::weak_ptr<StreamDesc>& weakDescr, unsigned int events)
{
    if(err)
    {
        return;
    }

    auto desc = weakDescr.lock();
    if(!desc)
    {
        return;
    }

    desc->events &= ~events;
    desc->collectedEvents |= events;
    if(!desc->eventHandlerPosted)
    {
        desc->eventHandlerPosted = true;
        boost::asio::post(desc->streamDescriptor.get_executor(), std::bind(&IOContextAdapter::adapterImpl::handleCollectEvents, this, weakDescr));
    }
}

void IOContextAdapter::adapterImpl::handleCollectEvents(const std::weak_ptr<StreamDesc>& weakDescr)
{
    auto descr(weakDescr.lock());
    if (!descr)
    {
        return;
    }

    auto collectedEvents = (descr->collectedEvents & ~ERROR_EVENT);
    auto hasErrorEvents = (descr->collectedEvents & ERROR_EVENT);

    descr->eventHandlerPosted = false;
    descr->collectedEvents = 0U;

    if(hasErrorEvents)
    {
        //判断是ERR、HUP 错误事件
        struct pollfd fds[] = {{descr->streamDescriptor.native_handle(), 0, 0}};

        const int ret = poll(fds, 1U, 0);
        if(ret == 1)
        {
            collectedEvents |= static_cast<unsigned int>(fds[0].revents);
        }
    }

    descr->eventHandler(collectedEvents);

    if(descr->streamDescriptor.is_open())
    {
        waitAsync(descr);
    }

}

IOContextAdapter::IOContextAdapter(boost::asio::io_context& ioc):impl(std::make_unique<adapterImpl>(ioc))
{

}

std::shared_ptr<ComAPIEventLoop> IOContextAdapter::getComAPIEventtLoop() const
{
    return impl->getComAPIEventLoop();
}

IOContextAdapter::~IOContextAdapter()
{

}
