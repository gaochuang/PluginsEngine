#include "testServerTest.hpp"
#include <iostream>

using namespace reactorFramework;
using namespace std;

TestTcpServer::TestTcpServer(EventLoop* loop, SocketAddr& addr) : TcpServer(loop, addr)
{

}

void TestTcpServer::connectCallback(std::shared_ptr<TcpConnect> tcpConnect)
{
    cout<<"new connect:"<<tcpConnect->getAddr().toString() <<"<count>" <<getConnectCount()<<endl;
}

void TestTcpServer::messageCallback(std::shared_ptr<TcpConnect> tcpConnect, Buffer& buffer)
{
    cout<<"thread id:"<<std::this_thread::get_id()<<endl;
    string addr = tcpConnect->getAddr().toString();
    string data;

    buffer.readAllAsString(data);

    cout<<"receive data form "<<addr<<":"<<data<<endl;
    tcpConnect->write(data);
}

void TestTcpServer::writeCompleteCallback(std::shared_ptr<TcpConnect> tcpConnect)
{
    cout<<"thread id:"<<std::this_thread::get_id()<<endl;
    string addr = tcpConnect->getAddr().toString();
    cout<<addr<<":"<<"write complet "<<endl;
}

void TestTcpServer::connectCloseCallback(std::shared_ptr<TcpConnect> connect)
{
    string addr = connect->getAddr().toString();
    cout<<"close connect :"<<addr<<endl;
}
