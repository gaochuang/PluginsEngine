#include "engine/engine.hpp"
#include "event/eventfd.hpp"
#include "plugin/FdMonitor.hpp"

#include <memory>

#include <iostream>
using namespace commonApi;

const unsigned int FdMonitor::EVENT_IN(EPOLLIN);
const unsigned int FdMonitor::EVENT_OUT(EPOLLOUT);
const unsigned int FdMonitor::EVENT_ERR(EPOLLERR);
const unsigned int FdMonitor::EVENT_HUP(EPOLLHUP);

const unsigned int ComAPIEventLoop::EVENT_IN(EPOLLIN);
const unsigned int ComAPIEventLoop::EVENT_OUT(EPOLLOUT);
const unsigned int ComAPIEventLoop::EVENT_ERR(EPOLLERR);
const unsigned int ComAPIEventLoop::EVENT_HUP(EPOLLHUP);

Engine::Engine(): loop(std::make_shared<EventLoop>())
{

}

Engine::Engine(const Callbacks& callbacks): callbacks(callbacks),loop(std::make_shared<EventLoop>())
{

}

Engine::~Engine()
{

}


void Engine::addFd(int fd, unsigned int events, const FdEventHandler& handler)
{
    auto event = std::make_shared<Event>(loop.get(), fd);
    if (events == FdMonitor::EVENT_IN)
    {
        event->setReadCallback(handler);
        event->enableReading(true);
    }

    if (events == FdMonitor::EVENT_OUT)
    {
        event->setWriteCallback(handler);
        event->enableWriting(true);
    }

    if (events == FdMonitor::EVENT_ERR)
    {
        event->setErrorCallback(handler);
        event->enableErrorEvent(true);
    }

    loop->addEvent(event);
}

void Engine::removeFd(int fd)
{
    loop->removeEvent(fd);
}

void Engine::modifyFd(int fd, unsigned int events)
{
    auto event = std::make_shared<Event>(loop.get(), fd);
    if (events == FdMonitor::EVENT_IN)
    {
        event->enableReading(true);
    }

    if (events == FdMonitor::EVENT_OUT)
    {
        event->enableWriting(true);
    }

    if (events == FdMonitor::EVENT_ERR)
    {
        event->enableErrorEvent(true);
    }

    loop->modifyEvent(event);
}

CallbackQueueService& Engine::getEventCallbackQueue()
{
    if(!callbackQueue)
    {
        callbackQueue = std::make_unique<EventFD>(loop.get());
    }

    return *callbackQueue;
}

TimerService& Engine::getTimerService()
{
    if (!timerQueue)
    {
        timerQueue = std::make_unique<TimerQueue>(loop.get());
    }

    return *timerQueue;
}

void Engine::run()
{
    loop->run();
}

void Engine::stop()
{
    loop->stop();
}

SignalMonitorService& Engine::getSignalMonitor()
{
    if(!signalMonitor)
    {
        signalMonitor = std::make_unique<SignalMonitor>(loop.get());
    }

    return *signalMonitor;
}

FdMonitor& Engine::getFdMonitor()
{
    return *this;
}

PluginServices::PluginList Engine::getExistingPlugins()
{
    PluginList ret;
    for(auto& it : pluginMap)
    {
        if(auto plugin = it.second.lock())
        {
            ret.push_back(plugin);
        }
    }
    
    return ret;
}

PluginDataStorage& Engine::getPluginDataStorage()
{
    return *this;
}

int Engine::getEpollFd() const
{
    return loop->getEpollFd();
}

void Engine::postCallback(const Callback& callback)
{
    getEventCallbackQueue().post(callback);
}

void Engine::postCallback(Callback&& callback)
{
    getEventCallbackQueue().post(std::move(callback));
}

void Engine::waitAndHandleEvents()
{
    loop->handleEvent(-1);
}

bool Engine::waitAndHandleEvents(const std::chrono::steady_clock::duration& timeout)
{
    if(timeout <= std::chrono::milliseconds(0))
    {
        loop->handleEvent(0);
    }
    const auto timeoutMs(std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count());
    loop->handleEvent(timeoutMs > std::numeric_limits<int>::max() ? std::numeric_limits<int>::max() : timeoutMs);

    return true;
}


std::shared_ptr<ComAPI> Engine::getComAPI()
{
    return shared_from_this();
}

void Engine::save(Key key, Data data)
{
    const auto ret = pluginDataMap.insert(std::make_pair(key, data));
    //如果插入失败，将已经存在的数据替换
    if(!ret.second)
    {
        ret.first->second = data;
    }
}

PluginDataStorage::Data Engine::get(Key key) const
{
    const auto it = pluginDataMap.find(key);
    if(it  == pluginDataMap.end())
    {
        return 0U;
    }

    return it->second;
}

void Engine::remove(Key key)
{
    pluginDataMap.erase(key);
}

void Engine::addSharedLibrary(const SharedLibraryMap::key_type& key, const std::shared_ptr<SharedLibrary>& sharedLibrary)
{
    sharedLibraryMap.add(key, sharedLibrary);
}

std::shared_ptr<SharedLibrary> Engine::getSharedLibrary(const SharedLibraryMap::key_type& key) const
{
    return sharedLibraryMap.get(key);
}

void Engine::updatePluginMap(const PluginMap::key_type& key, const std::shared_ptr<Plugin>& plugin)
{
    pluginMap.updatePlugin(key, plugin);
}

std::shared_ptr<ComAPIEventLoop> Engine::getComAPIEventLoop()
{
 return std::dynamic_pointer_cast<ComAPIEventLoop>(shared_from_this());
}

std::shared_ptr<ComAPI> ComAPI::create()
{
    auto engine = std::make_shared<Engine>();
    engine->getEventCallbackQueue();
    return engine;
}

std::shared_ptr<ComAPIEventLoop> ComAPIEventLoop::create(const Callbacks& callbacks)
{
    auto engine = std::make_shared<Engine>(callbacks);
    engine->getEventCallbackQueue();
    return engine;
}
