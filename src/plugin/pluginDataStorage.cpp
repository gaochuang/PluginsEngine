#include "pluginDataStorage.hpp"

#include <atomic>

using namespace reactorFramework;

PluginDataStorage::Key PluginDataStorage::getUniqueKey()
{
    static std::atomic<Key> key(0U);
    return key++;
}
