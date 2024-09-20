#include "plugin/PluginDataStorage.hpp"

#include <atomic>

using namespace commonApi;

PluginDataStorage::Key PluginDataStorage::getUniqueKey()
{
    static std::atomic<Key> key(0U);
    return key++;
}
