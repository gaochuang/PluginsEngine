#ifndef TCP_SERVER_Test_HPP
#define TCP_SERVER_Test_HPP

#include "tcpServer.hpp"

class TestTcpServer : public commonApi::TcpServer
{
public:
    TestTcpServer(commonApi::EventLoop* loop, commonApi::SocketAddr& addr);

    //tcpConnect可能关闭后内存被自动释放，传shared_ptr安全
    virtual void connectCallback(std::shared_ptr<commonApi::TcpConnect> tcpConnect);
    virtual void messageCallback(std::shared_ptr<commonApi::TcpConnect>, commonApi::Buffer&);
    virtual void writeCompleteCallback(std::shared_ptr<commonApi::TcpConnect> tcpConnect) ;
    virtual void connectCloseCallback( std::shared_ptr<commonApi::TcpConnect>);
};

#endif
