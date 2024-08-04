#include "tcpServer.hpp"

#include <iostream>

namespace reactorFramework
{

TcpServer::TcpServer(EventLoop* loop, SocketAddr& addr):eventLoop(loop),tcpAddr(addr),
                                                       tcpAccept(std::make_shared<TcpAccept>(loop, addr)),
                                                       threadPool(std::make_shared<EventThreadPool>(loop)),
                                                       isStart(false)
{
    tcpAccept->setConnectCallback(std::bind(&TcpServer::newConnected,this,std::placeholders::_1,std::placeholders::_2));

}

void TcpServer::start()
{
    //线程池初始化
    threadPool->init();
    tcpAccept->listen();
    isStart = true;
}

void TcpServer::setThreadPoolSize(uint16_t num)
{
    threadPool->setThreadNum(num);
}

void TcpServer::newConnected(int sockfd, const SocketAddr& addr)
{

    std::cout << "new tcp connect addr: " << addr.toString() << "count: " << std::to_string(getConnectCount()) <<std::endl;

    //从线程池获取一个线程处理新的连接
    EventLoop* loop = threadPool->getOneLoopFromPool();

    //创建一个新的连接对象
    auto tcpConnect = std::make_shared<TcpConnect>(loop, addr.getAddr(), sockfd);

    addConnect(addr.toString(), tcpConnect);

    //messageCallback 由用户自己实现
    tcpConnect->setMessageCallback(std::bind(&TcpServer::messageCallback, this, std::placeholders::_1, std::placeholders::_2));
    tcpConnect->setCloseCallback(std::bind(&TcpServer::connectCloseEvent,this,std::placeholders::_1));
    tcpConnect->connectedHandle();

    //调用connectCallback,由使用框架用户自己实现 void TestTcpServer::connectCallback(std::shared_ptr<TcpConnect> tcpConnect)
    connectCallback(tcpConnect);

}

void TcpServer::addConnect(const std::string& name, std::shared_ptr<TcpConnect> connect)
{
    connectPool.insert(std::pair<std::string, std::shared_ptr<TcpConnect>>(name, connect));
}

void TcpServer::addConnect(const std::string& name, TcpConnect* connect)
{
    std::shared_ptr<TcpConnect> connectPtr(connect);
    addConnect(name, connectPtr);
}

bool TcpServer::haveConnect(const std::string& name) const
{
    return (connectPool.find(name) != connectPool.end());
}

void TcpServer::removeConnect(const std::string& name)
{
    connectPool.erase(name);
}

long TcpServer::getConnectCount() const
{
    return connectPool.size();
}

void TcpServer::write(TcpConnect& connect, void* data, uint32_t length)
{
    connect.writeInLoop(data, length);
}

void TcpServer::write(const std::string& name, void* data, uint32_t length)
{
    if(haveConnect(name))
    {
        write(connectPool[name], data, length);
    }
    else
    {
        std::cerr << "no this connect, connect name: " << name <<std::endl;
    }
}

void TcpServer::write(std::shared_ptr<TcpConnect> connect, void* data, uint32_t length)
{
    connect->write(data, length);
}

void TcpServer::write(const std::string& name, const std::string& data)
{
    write(name, (void*)data.data(), data.length());
}

void TcpServer::connectCloseEvent(std::shared_ptr<TcpConnect> connect)
{
    //调用close callback
    connectCloseCallback(connect);
    removeConnect(connect->getName());
}

}
