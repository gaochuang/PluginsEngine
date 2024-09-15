#ifndef PLUGIN_SERVICES_HPP
#define PLUGIN_SERVICES_HPP

#include <list>
#include <memory>

namespace reactorFramework
{
class Plugin;
class CallbackQueueService;
class TimerService;
class signalMonitorService;
class FdMonitor;
class ComAPI;
class ComAPIEventLoop;
class PluginDataStorage;

class PluginServices
{
public:
    using PluginList = std::list<std::shared_ptr<Plugin>>;

    virtual ~PluginServices() = default;

    virtual CallbackQueueService& getEventCallbackQueue() = 0;
    virtual TimerService& getTimerService() = 0;
    virtual signalMonitorService& getSignalMonitor() = 0;
    virtual FdMonitor& getFdMonitor() = 0;
    virtual PluginList getExistingPlugins() = 0;
    virtual PluginDataStorage& getPluginDataStorage() = 0;

    virtual std::shared_ptr<ComAPI> getComAPI() = 0;
    virtual std::shared_ptr<ComAPIEventLoop> getComAPIEventLoop() = 0;

    virtual void run() = 0;
    virtual void stop() = 0;

    PluginServices(const PluginServices&) = delete;
    PluginServices(PluginServices&&) = delete;
    PluginServices& operator=(const PluginServices&) = delete;
    PluginServices& operator=(PluginServices&&) = delete;
protected:
    PluginServices() = default;
};

}

#endif
