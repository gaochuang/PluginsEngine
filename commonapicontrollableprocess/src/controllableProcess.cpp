#include "controllableProcess.hpp"
#include "controllableProcessPlugin.hpp"

#include <mutex>
#include <ostream>
#include <sstream>
#include <iostream>

using namespace reactorFramework::controllableprocess;

namespace
{
    std::recursive_mutex mutex;
    const ControllableProcess* createdInstance(nullptr);
}

ControllableProcess::~ControllableProcess()
{
    std::lock_guard<std::recursive_mutex> guard(mutex);
    if(this == createdInstance)
    {
        createdInstance = nullptr;
    }
}

std::shared_ptr<ControllableProcess> create(std::shared_ptr<reactorFramework::ComAPIEventLoop> ga)
{
    std::lock_guard<std::recursive_mutex> guard(mutex);

    if(nullptr != createdInstance)
    {
        std::ostringstream oss;
        oss << "ControllableProcess instance has already been created for this process" <<  std::endl;
        throw oss.str();
    }

    auto ret = reactorFramework::Plugin::create<ControllableProcess>(ga, COMMONAPICONTROLLABLEPROCESS_PLUGIN_CREATOR_NAME_STR);

    createdInstance = ret.get();

    return ret;

}

