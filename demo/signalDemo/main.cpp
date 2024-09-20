#include <iostream>
#include <eventLoop.hpp>
#include <functional>

#include <signal.h>

#include "engine.hpp"

#include "signalMonitorService.hpp"

using namespace std;

using namespace commonApi;



int main()
{
    Engine engine;
    

    SignalMonitorService& signalMonitor = engine.getSignalMonitor();

    // 使用lambda捕获loop的引用，并传递给信号处理函数
    signalMonitor.add(SIGABRT, [&engine]() {
        std::cout << "SIGABRT received" << std::endl;
        engine.stop();
    });

    signalMonitor.add(SIGTERM, [&engine]() {
        std::cout << "SIGTERM received" << std::endl;
        engine.stop();
    });

    engine.run();

    return 0;
}
