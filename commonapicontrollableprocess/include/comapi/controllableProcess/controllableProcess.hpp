#ifndef COMMON_API_CONTROLLABLE_PROCESS_HPP_
#define COMMON_API_CONTROLLABLE_PROCESS_HPP_

#include <functional>
#include <memory>

#include "plugin.hpp"

namespace reactorFramework
{
    class ComAPIEventLoop;

    namespace controllableprocess
    {
        class ControllableProcess: public Plugin
        {
        public:
            using TerminateCb = std::function<void()>;

            virtual ~ControllableProcess();

            virtual void setTerminateCb(const TerminateCb& cb) = 0;

            static std::shared_ptr<ControllableProcess> create(std::shared_ptr<ComAPIEventLoop> ga);

            ControllableProcess(const ControllableProcess&) = delete;
            ControllableProcess(ControllableProcess&&) = delete;
            ControllableProcess& operator=(const ControllableProcess&) = delete;
            ControllableProcess& operator=(ControllableProcess&&) = delete;
        protected:
            ControllableProcess() = default;
        };
    }
}

#endif
