#ifndef IO_CONTEXT_ADAPTET_HPP
#define IO_CONTEXT_ADAPTET_HPP

#include "ComAPIEventLoop.hpp"

#include <boost/asio/io_context.hpp>
#include <memory>

namespace commonApi
{
    class ComAPIEventLoop;

    class IOContextAdapter
    {
    public:
        explicit IOContextAdapter(boost::asio::io_context& ioc);
        ~IOContextAdapter();

        IOContextAdapter(const IOContextAdapter&) = delete;
        IOContextAdapter(IOContextAdapter&& ) = delete;
        IOContextAdapter& operator=(const IOContextAdapter&) = delete;
        IOContextAdapter& operator=(IOContextAdapter&) = delete;


        std::shared_ptr<ComAPIEventLoop> getComAPIEventtLoop() const;
    private:
        class adapterImpl;
        std::unique_ptr<adapterImpl> impl;
    };
}

#endif
