#include "testServerTest.hpp"
#include <iostream>
#include "eventLoop.hpp"
#include "eventThreadPool.hpp"

using namespace std;
using namespace reactorFramework;

int main()
{
    EventLoop loop;

    //监听地址和端口
    SocketAddr addr(true, 20000);

    //创建服务器
    TestTcpServer server(&loop, addr);

    //设置线程池大小
  //  server.setThreadPoolSize(3);

    //服务器开始运行
    server.start();

    //事件循环机制运行
    loop.run();

    return 0;
}
