#include <iostream>
#include <eventLoop.hpp>
#include <functional>
#include <signalMonitor.hpp>
#include <signal.h>

using namespace std;

using namespace reactorFramework;

void handleSigAbort() 
{
    std::cout << "SIGABRT received" << std::endl;
}

void handleSigTerm() 
{
    std::cout << "SIGTERM received" << std::endl;
}

int main()
{
    EventLoop loop;

    SignalMonitor sig(&loop);

    sig.add(SIGABRT, handleSigAbort);
    sig.add(SIGTERM, handleSigTerm);

    loop.run();

    return 0;
}
