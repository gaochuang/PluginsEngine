#include <iostream>
#include <eventLoop.hpp>
#include <functional>
#include <signalMonitor.hpp>
#include <signal.h>

using namespace std;

using namespace reactorFramework;



int main()
{
    EventLoop loop;

    SignalMonitor sig(&loop);

    // 使用lambda捕获loop的引用，并传递给信号处理函数
    sig.add(SIGABRT, [&loop]() {
        std::cout << "SIGABRT received" << std::endl;
        loop.stop();
    });

    sig.add(SIGTERM, [&loop]() {
        std::cout << "SIGTERM received" << std::endl;
        loop.stop();
    });

    loop.run();

    return 0;
}
