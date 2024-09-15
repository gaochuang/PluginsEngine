#ifndef SHARED_LIBRARY_MAP_HPP
#define SHARED_LIBRARY_MAP_HPP


#include <string>
#include <memory>

#include "sharedLibrary.hpp"
#include "utilMap.hpp"

namespace reactorFramework
{
    using SharedLibraryMap = Map<std::string, std::shared_ptr<SharedLibrary>>;
}

#endif
