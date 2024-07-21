#include "tcpAccept.hpp"
#include "socketOperation.hpp"
#include <memory>

namespace reactorFramework
{

TcpAccept::TcpAccept(EventLoop* loop, SocketAddr& addr):eventLoop(loop),
      socket(std::make_shared<Socket>(SocketOperation::createNonblockingSocket())),
      event(std::make_shared<Event>(loop, socket->getSocketFd())),
      listening(false)
{
    loop->addEvent(event);
    socket->bind(addr);
    event->setReadCallback(std::bind(&TcpAccept::acceptHandle, this));
}

TcpAccept::~TcpAccept()
{
    event->disableAll();
    event->removeFromLoop();
}

void TcpAccept::listen()
{
    socket->listen();
    event->enableReading(true);
}

bool TcpAccept::isListen()
{
    return listening;
}

void TcpAccept::setConnectCallback(const connectCallBack callback)
{
    this->callBack = callBack;
}

void TcpAccept::acceptHandle()
{
    SocketAddr connectAddr;
    auto fd = socket->accept(connectAddr);
    if( fd > 0)
    {
        if(callBack)
        {
            callBack(fd, connectAddr);
        }else 
        {
           ::close(fd);
        }
    }

}

}
