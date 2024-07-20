#include "socket.hpp"
#include "socketOperation.hpp"

namespace reactorFramework
{

Socket::Socket(int fd):socketFd(fd)
{

}

Socket::~Socket()
{
    SocketOperation::close(socketFd);
}

void Socket::bind(SocketAddr& addr)
{
    SocketOperation::bind(socketFd, addr.getAddrPtr());
}

int Socket::accept(SocketAddr& addr) const
{
    struct sockaddr_in clientAddr;
    auto ret = SocketOperation::accept(socketFd, &clientAddr);
    if(ret > 0)
    {
        addr.setAddr(clientAddr);
    }

    return ret;
}

void Socket::listen()
{
    SocketOperation::listen(socketFd);
}

int Socket::getSocketFd() const
{
    return socketFd;
}

void Socket::setTcpNoDelay(bool enable)
{
    SocketOperation::setTcpNoDelay(socketFd, enable);
}

int Socket::shutdownWrite()
{
    SocketOperation::shutdownWrite(socketFd);
}


}

