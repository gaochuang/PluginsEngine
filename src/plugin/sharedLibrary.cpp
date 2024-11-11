#include <dlfcn.h>
#include <iostream>
#include <cerrno>    // errno
#include <cstring>   // strerror
#include "engine/sharedLibrary.hpp"

namespace commonApi
{

std::string getLastError()
{ 
    const auto str(::dlerror());
    if(nullptr != str)
    {
        return str;
    }

    return {};
}

SharedLibrary::SharedLibrary(const char* fileName)
{
    handle = ::dlopen(fileName, RTLD_NOW | RTLD_LOCAL);
    if(nullptr == handle)
    {
        std::cerr << "open  "<< fileName << " failed , handle is null, err: " << ::dlerror() << std::endl;
    }

    lastError = getLastError();
}

SharedLibrary::~SharedLibrary() 
{

}

bool SharedLibrary::valid() const
{
    return handle != nullptr;
}

void* SharedLibrary::dlsym(const std::string& symbolName)
{
    if(nullptr == handle)
    {
        return nullptr;
    }

    const auto ret(::dlsym(handle, symbolName.c_str()));
    lastError = getLastError();
    return ret;
}

} // namespace commonApi
