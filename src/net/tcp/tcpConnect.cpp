#include "tcpConnect.hpp"
#include "socketOperation.hpp"

#include <memory>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>

namespace reactorFramework
{


TcpConnect::TcpConnect(EventLoop* loop, struct sockaddr_in addr, int fd):loop(loop),
            socketAddr(addr),
            name(socketAddr.toString()),
            socket(std::make_shared<Socket>(fd)),
            event(std::make_shared<Event>(loop, fd)),
            state(Disconnected)
{

    setNoDelay(true);
    loop->addEvent(event);
    event->setReadCallback(std::bind(&TcpConnect::readEvent, this));
    event->setWriteCallback(std::bind(&TcpConnect::writeEvent, this));
    event->setErrorCallback(std::bind(&TcpConnect::errorEvent, this));
    event->setCloseCallback(std::bind(&TcpConnect::closeEvent, this));

}

TcpConnect::~TcpConnect()
{
    event->disableAll();
    event->removeFromLoop();
}

void TcpConnect::setNoDelay(bool enable)
{
    socket->setTcpNoDelay(enable);
}

void TcpConnect::setMessageCallback(const oneMessageCallback callback)
{
    messageCallback = callback;
}

void TcpConnect::setCloseCallback(const oneCloseCallback callback)
{
    closeCallback = callback;
}

void TcpConnect::setWriteCompleteCallback(const oneWriteCompleteCallback callback)
{
    writeCompleteCallback = callback;
}

void TcpConnect::readEvent()
{
    int error(0);
    auto n = readBuf.readFromIO(event->getFd(), error);
    if (n > 0)
    {
        //调用用户自己实现的callback
        if(messageCallback)
        {
            messageCallback(shared_from_this(), readBuf);
        }
    }else if(n == 0)
    {

        std::cout << "will call closeEvent" << std::endl;
        closeEvent();
    }else
    {
        //但是buffer没有数据
        if(errno == EWOULDBLOCK)
        {
            return;
        }
        std::cerr << "TcpConnection::handleRead error :" << ". Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;
        closeEvent();
    }
}


void TcpConnect::writeEvent()
{
    if (event->isWriting())
    {
        std::cerr << "connect failed, socket fd " << std::to_string(event->getFd()) << ". Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;
        return;
    }

    auto n = SocketOperation::write(event->getFd(), writeBuf.readIndexPtr(), writeBuf.readableBytes());
    if(n > 0)
    {
        writeBuf.clearReadIndex(n);
        if(writeBuf.isEmpty())
        {
            event->enableWriting(false);
            if(writeCompleteCallback)
                
                writeCompleteCallback(shared_from_this()); 
        }
 
    }else
    {
        //这里应该是写错误，处理写错误
        if(errno == EWOULDBLOCK )
        {
            return;
        }

        std::cerr << "Write data error. Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;
        // 处理其他写错误，关闭连接

        std::cout << "will call closeEvent" << std::endl;
        closeEvent();
    }
}

void TcpConnect::closeEvent()
{
    if(state == Disconnected)
    {
        return;
    }
    state = Disconnected;
    if(closeCallback)
    {
        closeCallback(shared_from_this());
    }
}

TcpConnect& TcpConnect::getReference()
{
    return (*this);
}

const SocketAddr& TcpConnect::getAddr() const
{
    return socketAddr;
}

std::string TcpConnect::getName() const
{
    return name;
}

void TcpConnect::connectedHandle()
{
    state = Connected;
    event->enableReading(true);
    event->enableErrorEvent(true);
}

void TcpConnect::errorEvent()
{
    std::cout <<"### will call closeEvent" << std::endl;
    closeEvent();
}

void TcpConnect::write(const char* data)
{
    write((void*)data, ::strlen(data));
}

void TcpConnect::write(const std::string& data)
{
    write(data.data(), data.length());
}

void TcpConnect::write(const void* data, uint32_t length)
{
    int n(0);
    size_t remaining(length);
    bool faultError(false);

    if(Disconnected == state)
    {
       std::cerr << "disconnected, need not write" << std::endl; 
       return;
    }
    //如该写数据缓冲区内有数据，直接写入socket缓冲区会导致数据交叠
    if(!event->isWriting() && writeBuf.isEmpty())
    {
        n = SocketOperation::write(event->getFd(), data, length);
        if(n >= 0)
        {
            remaining = length - n;
            if(remaining == 0 && writeCompleteCallback)
            {
                writeCompleteCallback(shared_from_this());
            }
        }else
        {
            n = 0;
            //写操作会阻塞，在非阻塞套接字上出现
            if(errno != EWOULDBLOCK)
            {
                std::cerr << "Write data error: " << std::strerror(errno) << " (" << errno << ")" << std::endl;

                if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN || errno == EINVAL)
                {
                    faultError = true;
                }
            }
        }
    }

    if (!faultError && remaining > 0)
    {
        writeBuf.append(static_cast<const char*>(data)+n, remaining);
        if (!event->isWriting())
        {
            event->enableWriting(true);
        }
    }

}

void TcpConnect::writeInLoop(const void* data, uint32_t length)
{
    loop->runInLoop([this,data,length]{
        write(data,length);
    });
}

void TcpConnect::shutdownWrite()
{
    if (state == Connected)
    {
        state = Disconnecting;
        socket->shutdownWrite();
    }
}

}

