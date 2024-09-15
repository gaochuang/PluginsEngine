#ifndef PLUGIN_DATA_STORAGE_HPP
#define PLUGIN_DATA_STORAGE_HPP

#include <cstdint>

namespace reactorFramework
{

class PluginDataStorage
{
public:
    using Key = uint32_t;
    using Data = uintptr_t;

    virtual ~PluginDataStorage() = default;
    PluginDataStorage(const PluginDataStorage&) = delete;
    PluginDataStorage(PluginDataStorage&&) = delete;
    PluginDataStorage& operator=(const PluginDataStorage&) = delete;
    PluginDataStorage& operator=(PluginDataStorage&&) = delete;

    virtual void save(Key key, Data data) = 0;
    virtual Data get(Key key) const = 0;
    virtual void remove(Key key) = 0;
    static Key getUniqueKey();

protected:
    PluginDataStorage() = default;



};

}

#endif

