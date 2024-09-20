#ifndef COMMON_API_PLUGIN_PLUGIN_HPP
#define COMMON_API_PLUGIN_PLUGIN_HPP

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>

namespace commonApi
{

class ComAPIEventLoop;
  
class Plugin
{
public:
    virtual ~Plugin();

    template <typename PluginType>
    static std::shared_ptr<PluginType> create(std::shared_ptr<ComAPIEventLoop> eventLoop, const std::string& functionName, const void* extraArgs = nullptr);

    Plugin(const Plugin&) = delete;
    Plugin(Plugin&&) = delete;
    Plugin& operator=(const Plugin&) = delete;
    Plugin& operator=(Plugin&&) = delete;
protected:
    Plugin();
private:
    static std::shared_ptr<Plugin> createImpl(
            std::shared_ptr<ComAPIEventLoop> eventLoop, const char* pluginTypeName, std::size_t key, const std::string& functionName, const void* extraArgs = nullptr);
};

template <typename PluginType>
std::shared_ptr<PluginType> Plugin::create(std::shared_ptr<ComAPIEventLoop> eventLoop, const std::string& functionName, const void* extraArgs)
{
    static_assert(std::is_base_of<Plugin, PluginType>::value);
    static_assert(std::is_convertible<PluginType*, Plugin*>::value);

    //当指针是智能指针时候，向下转换
    auto ret(std::dynamic_pointer_cast<PluginType>(createImpl(eventLoop, typeid(PluginType).name(), typeid(PluginType).hash_code(), functionName, extraArgs)));
    if (ret == nullptr)
    {
        abort();
    }
    return ret;
}

}

#endif
