#include "socketAddr.hpp"
#include "socketOperation.hpp"

namespace reactorFramework
{

SocketAddr::SocketAddr():valid(false)
{
}

SocketAddr::SocketAddr(bool isAny, int port)
{
    if(isAny)
    {
        SocketOperation::getAddrAnyIpv4(sockAddr, port);
    }
}

SocketAddr::SocketAddr(struct sockaddr_in addr): valid(true), sockAddr(addr)
{

}

SocketAddr::SocketAddr(const std::string& addrPort):valid(false)
{
    struct sockaddr_in addrIn;
    if(!SocketOperation::toAddrIpv4(addrPort,addrIn))
    {
        return ;
    }
    *this = SocketAddr(addrIn);
}

SocketAddr::SocketAddr(const std::string& addr, uint16_t port):valid(false)
{
    struct sockaddr_in addrIn;
    if(!SocketOperation::toAddrIpv4(addr,port,addrIn))
    {
        return ;
    }

    *this = SocketAddr(addrIn);
}

SocketAddr::SocketAddr(uint16_t port):valid(false)
{
    struct sockaddr_in addrIn;
    if(!SocketOperation::toAddrIpv4(port,addrIn))
    {
        return ;
    }

    *this = SocketAddr(addrIn);
}


SocketAddr::~SocketAddr()
{
}

void SocketAddr::setAddr(struct sockaddr_in addr)
{
    sockAddr = addr;
    valid = true;
}

struct sockaddr_in SocketAddr::getAddr() const
{
    return sockAddr;
}

struct sockaddr_in* SocketAddr::getAddrPtr()
{
    return &sockAddr;
}

bool SocketAddr::isValid()
{
    return valid;
}

std::string SocketAddr::toString() const
{
    SocketOperation::toString(sockAddr);
}

}
