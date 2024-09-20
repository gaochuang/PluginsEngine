#ifndef COMMON_API_PLUGIN_PLUGIN_CREATOR_HPP
#define COMMON_API_PLUGIN_PLUGIN_CREATOR_HPP

#include <cstdarg>
#include <memory>
#include <string>
#include <vector>

namespace commonApi
{

class Plugin;
class SharedLibrary;
class Engine;

class PluginCreator
{
public:
    static std::vector<std::string> colonSeparatedStringToPaths(const std::string& str);

    static std::shared_ptr<Plugin> create(const std::shared_ptr<Engine>& engine,
                                              const char* pluginTypeName,
                                              std::size_t key,
                                              const std::string& functionName,
                                              const void* extraArgs = nullptr);

    static std::shared_ptr<Plugin> createFromDirectory(const std::vector<std::string>& blacklist,
                                                           const std::string& path,
                                                           const std::shared_ptr<Engine>& engine,
                                                           const char* pluginTypeName,
                                                           const std::string& functionName,
                                                           const void* extraArgs = nullptr);

    static std::shared_ptr<Plugin> createFromFile(const std::vector<std::string>& blacklist,
                                                      const std::string& path,
                                                      const std::shared_ptr<Engine>& engine,
                                                      const std::string& functionName,
                                                      const void* extraArgs = nullptr);

    static std::shared_ptr<Plugin> createFromSharedLibrary(const std::shared_ptr<SharedLibrary>& sharedLibrary,
                                                               const std::string& path,
                                                               const std::shared_ptr<Engine>& engine,
                                                               const std::string& functionName,
                                                               const void* extraArgs = nullptr);

    PluginCreator() = delete;
    PluginCreator(const PluginCreator&) = delete;
    PluginCreator(PluginCreator&&) = delete;
    PluginCreator& operator=(const PluginCreator&) = delete;
    PluginCreator& operator=(PluginCreator&&) = delete;
};

}
#endif
