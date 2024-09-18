#ifndef COMMON_API_PLUGIN_DEFINES_HPP_
#define COMMON_API_PLUGIN_DEFINES_HPP_

#include <memory>
#include <type_traits>

namespace commonapi
{
    class Plugin;
    class PluginServices;
}

//确保跨编译器的一致性
#ifdef __clang__
#define COMMON_API_NO_NAME_MANGLING(_function)                                                \
    _Pragma("clang diagnostic push")                                                          \
        _Pragma("clang diagnostic ignored \"-Wreturn-type-c-linkage\"") extern "C" _function  \
            _Pragma("clang diagnostic pop")
#else
#define COMMON_API_NO_NAME_MANGLING(_function) extern "C" _function
#endif

#define COMMONAPI_DECLARE_PLUGIN_CREATOR(_plugintype, _funcname, _services)        \
    static_assert(std::is_base_of<::reactorFramework::Plugin, _plugintype>::value);       \
    static_assert(std::is_convertible<_plugintype*, ::reactorFramework::Plugin*>::value); \
    COMMONAPI_NO_NAME_MANGLING(::std::shared_ptr<::reactorFramework::Plugin> _funcname(::std::shared_ptr<::reactorFramework::PluginServices> _services, const void*))

#define COMMONAPI_DECLARE_PLUGIN_CREATOR_WITH_ARGS(_plugintype, _funcname, _services, ...)                                                         \
    static_assert(std::is_base_of<::reactorFramework::Plugin, _plugintype>::value);                                                                       \
    static_assert(std::is_convertible<_plugintype*, ::reactorFramework::Plugin*>::value);                                                                 \
    COMMONAPI_NO_NAME_MANGLING(::std::shared_ptr<::reactorFramework::Plugin> _funcname(::std::shared_ptr<::reactorFramework::PluginServices> _services, const void*)); \
    COMMONAPI_NO_NAME_MANGLING(::std::shared_ptr<::reactorFramework::Plugin> COMMONAPI_MAKE_CREATOR_IMPL_NAME(_funcname)(::std::shared_ptr<::reactorFramework::PluginServices> _services, __VA_ARGS__))

#define COMMONAPI_DEFINE_PLUGIN_CREATOR(_plugintype, _funcname, _services) \
    ::std::shared_ptr<::reactorFramework::Plugin> _funcname(::std::shared_ptr<::reactorFramework::PluginServices> _services, const void*)


#define COMMONAPI_DEFINE_PLUGIN_CREATOR_WITH_ARGS(_plugintype, _funcname, _services, ...) \
    ::std::shared_ptr<::reactorFramework::Plugin> _funcname(::std::shared_ptr<::reactorFramework::PluginServices> _services, __VA_ARGS__)

#define COMMONAPI_XSTR(s) COMMONAPI_STR(s)
#define COMMONAPI_STR(s) #s

#define COMMONAPI_CONCAT(A, B) A##B


#define COMMONAPI_MAKE_CREATOR_IMPL_NAME(N) COMMONAPI_STR(N, _impl)

#endif
