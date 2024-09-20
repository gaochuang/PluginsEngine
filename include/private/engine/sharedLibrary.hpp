#ifndef SHAREDLIBRARY_HPP
#define SHAREDLIBRARY_HPP

#include <functional>
#include <string>

namespace commonApi
{

class SharedLibrary
{

public:
    explicit SharedLibrary(const char* fileName);

    ~SharedLibrary();

    bool valid() const;
    const std::string& getLastError() const { return lastError; }

    template <typename ReturnType, typename... Params>
    std::function<ReturnType(Params...)> resolveFunction(const std::string& symbolName);

    SharedLibrary(const SharedLibrary&) = delete;
    SharedLibrary(SharedLibrary&&) = delete;  
    SharedLibrary& operator=(const SharedLibrary&) = delete;
    SharedLibrary& operator=(SharedLibrary&&) = delete;

 private:
    void* dlsym(const std::string& symbolName);
    void* handle;
    std::string lastError;
};

template <typename ReturnType, typename... Params>
std::function<ReturnType(Params...)> SharedLibrary::resolveFunction(const std::string& symbolName)
{
    using FunctionType = ReturnType(Params...);
    return std::function<FunctionType>(reinterpret_cast<FunctionType*>(dlsym(symbolName)));
}

}

#endif
