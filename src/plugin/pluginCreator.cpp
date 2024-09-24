#include "plugin/PluginCreator.hpp"
#include "engine/engine.hpp" // Include the header file for the 'Engine' class

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <filesystem>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using namespace commonApi;

#define COMMON_API_PLUGINS "/usr/local/libexec"

namespace
{

using Paths = std::vector<std::string>;
Paths listDirectory(const std::string& path)
{
    const int savedErrno(errno);
    Paths paths;
    try
    {
        for(auto const& it : std::filesystem::directory_iterator(path))
        {
            paths.push_back(it.path().string());
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        if(e.code() == std::errc::no_such_file_or_directory)
        {
            std::cerr << "No such file or directory: " << path << std::endl;
        }else
        {
            std::ostringstream oss;
            oss << "Error listing directory " << path << ": " << e.what();
            throw oss.str();
        }
    }

    //有多个plugin按照字母顺序
    std::sort(paths.begin(), paths.end());

    errno = savedErrno;
    return paths;
}

std::shared_ptr<SharedLibrary> getSharedLibrary(const std::string& path, Engine& engine)
{
    auto sharedLibrary = engine.getSharedLibrary(path);
    if (!sharedLibrary)
    {
        sharedLibrary = std::make_shared<SharedLibrary>(path.c_str());
    }

    return sharedLibrary;
}

void addToEngine(const std::string& path, Engine& engine, const std::shared_ptr<SharedLibrary>& sharedLibrary)
{
    engine.addSharedLibrary(path, sharedLibrary);
}

bool contains(const std::string& s, const std::vector<std::string>& v)
{
    for (const auto& i : v)
    {
        if (i == s)
        {
            return true;
        }
    }

    return false;
}

}

//input "path1:path2::path3"
//output {"path1", "path2", "path3"}
std::vector<std::string> PluginCreator:: colonSeparatedStringToPaths(const std::string& str)
{
    std::vector<std::string> ret;
    auto previous = str.begin();

    for (auto i = str.begin(); i != str.end(); ++i)
    {
        if (*i == ':')
        {
            const std::string candidate(previous, i);
            if (!candidate.empty())
                ret.push_back(candidate);
            previous = i + 1;
        }
    }
    
    if (previous != str.end())
    {
        const std::string candidate(previous, str.end());
        if (!candidate.empty())
            ret.push_back(candidate);
    }

    return ret;
}

std::shared_ptr<Plugin> PluginCreator::create(const std::shared_ptr<Engine>& engine,
                                              const char* pluginTypeName,
                                              std::size_t key,
                                              const std::string& functionName,
                                              const void* extraArgs)
{
    std::vector<std::string> blacklist;
   
    //可以通过环境遍历 COMAPI_PLUGINS_BLACKLIST_VAR_NAME 配置黑名单
    const char* blackListEnv = ::getenv("COMAPI_PLUGINS_BLACKLIST_VAR_NAME");
    if(nullptr != blackListEnv)
    {
        blacklist = colonSeparatedStringToPaths(blackListEnv);
    }else
    {
        std::cout << "No blacklist found" << std::endl;
    }

    //可以通过环境遍历 COMAPI_PLUGINS_PATH_VAR_NAME 配置插件路径
    const char* pluginsEnv = ::getenv("COMAPI_PLUGINS_PATH_VAR_NAME");
    if (nullptr != pluginsEnv)
    {
        const auto paths = colonSeparatedStringToPaths(pluginsEnv);
        for (const auto& it : paths)
        {
            if (auto plugin = createFromFile(blacklist, it, engine, functionName, extraArgs))
            {
                return plugin;
            }
        }
    }else
    {
        std::cout << "No plugins found " << "COMAPI_PLUGINS_PATH_VAR_NAME" << std::endl;
    }

    auto ret = createFromDirectory(blacklist, COMMON_API_PLUGINS, engine, pluginTypeName, functionName, extraArgs);
    if(nullptr == ret)
    {
        std::cerr << "createFromDirectory failed " << std::endl;
        return nullptr;
    }
    engine->updatePluginMap(key, ret);

    return ret;
}

std::shared_ptr<Plugin> PluginCreator::createFromDirectory(const std::vector<std::string>& blacklist,
                                                           const std::string& path,
                                                           const std::shared_ptr<Engine>& engine,
                                                           const char* pluginTypeName,
                                                           const std::string& functionName,
                                                           const void* extraArgs)
{
    const auto paths = listDirectory(path);
    for(auto const& it : paths)
    {
        if(auto plugin = createFromFile(blacklist, it, engine, functionName, extraArgs))
        {
            return plugin;
        }
    }

    std::ostringstream oss;
    oss << "No plugins found in " << path << pluginTypeName <<std::endl;
    throw oss.str();
}

std::shared_ptr<Plugin> PluginCreator::createFromFile(const std::vector<std::string>& blacklist,
                                                      const std::string& path,
                                                      const std::shared_ptr<Engine>& engine,
                                                      const std::string& functionName,
                                                      const void* extraArgs)
{
    if(contains(path, blacklist))
    {
        std::cout << "Plugin " << path << " is blacklisted" << std::endl;
        return nullptr;
    }

    auto sharedLibrary = getSharedLibrary(path, *engine);
    if(!sharedLibrary->valid())
    {
        std::cout << "Shared library " << path << " is invalid" << std::endl;
        return nullptr;
    }

    addToEngine(path, *engine, sharedLibrary);

    return createFromSharedLibrary(sharedLibrary, path, engine, functionName, extraArgs);
}

std::shared_ptr<Plugin> PluginCreator::createFromSharedLibrary(const std::shared_ptr<SharedLibrary>& sharedLibrary,
                                                               const std::string& path,
                                                               const std::shared_ptr<Engine>& engine,
                                                               const std::string& functionName,
                                                               const void* extraArgs)
{
    auto creator = sharedLibrary->resolveFunction<std::shared_ptr<Plugin>, std::shared_ptr<PluginServices>, const void*>(functionName);
    if(creator)
    {
        auto ret = creator(engine, extraArgs);
        if(!ret)
        {
            std::cerr << "Failed to create plugin from shared library " << path << std::endl;
        }else
        {
            std::cout << "Created plugin from shared library " << path << std::endl;
        }

        return ret;
    }

    std::cerr << "Failed to resolve function " << functionName << " in shared library " << path << "err: " << sharedLibrary->getLastError()<<std::endl;
    return nullptr;
}
