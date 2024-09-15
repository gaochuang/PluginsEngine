#include "testHttpServer.hpp"
#include <iostream>
#include "eventLoop.hpp"
#include "eventThreadPool.hpp"

using namespace reactorFramework;

int main()
{
    EventLoop loop;
    SocketAddr addr(true, 8081);
    TestHttpServer server(&loop, addr);
 //   server.setThreadPoolSize(2);
    server.start();
    loop.run();
}
