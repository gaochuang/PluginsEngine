#ifndef COMAPI_PLUGIN_GET_SHARE_PTR_HPP_
#define COMAPI_PLUGIN_GET_SHARE_PTR_HPP_

#include <memory>
#include "commonapi.h"

namespace commonApi
{
    class ComAPI;
    
    std::shared_ptr<ComAPI> getSharedPtr(comApi_handle handle);
}

#endif
