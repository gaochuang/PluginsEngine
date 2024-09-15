#include <cstdlib>
#include <cxxabi.h>
#include <sstream>
#include <string>
#include <vector>

#include "plugin.hpp"
#include "engine.hpp"
#include "pluginCreator.hpp"

using namespace reactorFramework;

Plugin::Plugin()
{

}

Plugin::~Plugin()
{

}

std::shared_ptr<Plugin> Plugin::createImpl(std::shared_ptr<ComAPIEventLoop> eventLoop, const char* pluginTypeName, std::size_t key, const std::string& functionName, const void* extraArgs)
{
    const std::shared_ptr<Engine> engine(std::dynamic_pointer_cast<Engine>(eventLoop));
    if (!engine)
    {
        abort();
    }

    return PluginCreator::create(engine, pluginTypeName, key, functionName, extraArgs);
}

