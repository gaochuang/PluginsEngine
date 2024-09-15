#ifndef PLGIN_MAP_HPP
#define PLGIN_MAP_HPP

#include "plugin.hpp"

namespace reactorFramework
{

class PluginMap
{
public:
    using Impl = std::multimap<std::size_t, std::weak_ptr<Plugin>>;
    using iterator = Impl::iterator;
    using key_type = Impl::key_type;
    using mapped_type = Impl::mapped_type;

    //遍历删除无效的插件
    void eraseReleasedPlugins(std::size_t key)
    {
        const auto range = pluginMap.equal_range(key);
        for (auto it = range.first; it != range.second;)
        {
            //通过调用lock()方法，返回一个shared_ptr对象，如果weak_ptr对象已经失效，返回一个空的shared_ptr对象
            auto plugin(it->second.lock());
            if (!plugin)
            {
                it = pluginMap.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void updatePlugin(std::size_t key, const std::shared_ptr<Plugin>& newPlugin)
    {
        eraseReleasedPlugins(key);
        const auto range = pluginMap.equal_range(key);
        for(auto it = range.first; it != range.second; ++it)
        {
            auto plugin(it->second.lock());
            if(plugin && plugin.get() == newPlugin.get())
            {
                return;
            }

        }
    
        pluginMap.insert(std::make_pair(key, newPlugin));
    }

    iterator begin()
    {
        return pluginMap.begin();
    }

    iterator end()
    {
        return pluginMap.end();
    }

    std::size_t size() const
    {
        return pluginMap.size();
    }

    iterator find(const key_type& key) 
    {
        return pluginMap.find(key);
    }

    PluginMap() = default;
    ~PluginMap() = default;
    PluginMap(const PluginMap&) = delete;
    PluginMap(PluginMap&&) = delete;
    PluginMap& operator=(const PluginMap&) = delete;
    PluginMap& operator=(PluginMap&&) = delete;
private:
    Impl pluginMap;
};

}
#endif
