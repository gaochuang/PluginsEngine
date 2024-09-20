#include "net/tcp/socketOperation.hpp"

#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <sstream>
#include <sys/uio.h>

namespace commonApi
{

const int32_t SocketOperation::Ipv4AddrAny = htonl(INADDR_ANY);

int SocketOperation::createNonblockingSocket()
{
    auto sockFd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,  IPPROTO_TCP);
    if(sockFd < 0)
    {
        std::cerr << "ailed to create socket " << ". Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;
    }

    return sockFd;
}

void SocketOperation::close(int sockfd)
{
    if(::close(sockfd) < 0)
    {
        std::cerr << "Failed to close socket fd " << sockfd << ". Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;
    }
}

int SocketOperation::shutdownWrite(int sockfd)
{
   auto ret = ::shutdown(sockfd, SHUT_WR);
   if(ret < 0)
   {
       std::cerr << "Failed to shut down write socket fd " << sockfd << ". Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;
   }

   return ret;
}

/* 是否启用Nagle算法
*/
void SocketOperation::setTcpNoDelay(int fd, bool isEnable)
{
    auto opt = isEnable ? 1 : 0;
    ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
}

int SocketOperation::bind(int sockfd, const struct sockaddr_in* addr)
{
    auto ret = ::bind(sockfd,  (struct sockaddr *)addr, (sizeof(struct sockaddr)));
    if(ret < 0)
    {
        std::cerr << "Failed to bind socket fd " << sockfd << ". Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;
    }

    return ret;
}

int SocketOperation::listen(int sockfd)
{
    auto ret = ::listen(sockfd, SOMAXCONN);
    if(ret < 0)
    {
        std::cerr << "Failed to listen socket fd " << sockfd << ". Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;
    }

    return ret;
}


int SocketOperation::connect(int sockfd, const struct sockaddr* addr)
{
    auto ret = ::connect(sockfd, addr, sizeof(struct sockaddr));
    if(ret < 0)
    {
        std::cerr << "Failed to connect socket fd " << sockfd << ". Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;  
    }

    return ret;
}

int SocketOperation::accept(int sockfd, struct sockaddr_in* addr)
{
	socklen_t len = sizeof(struct sockaddr_in);
    auto ret = ::accept(sockfd, (struct sockaddr*)addr, &len);
    if(ret < 0)
    {
      std::cerr << "accept failed socket fd: " << sockfd << ". Error code: " << errno << " (" << std::strerror(errno) << ")" << std::endl;  
    }

    return ret;
}

ssize_t SocketOperation::read(int sockfd, void *buf, size_t count)
{
   return TEMP_FAILURE_RETRY(::read(sockfd, buf, count));
}

/*
散布读，将文件中若干连续的数据块读入内存分散的缓冲区中
*/
ssize_t SocketOperation::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    return TEMP_FAILURE_RETRY(::readv(sockfd, iov, iovcnt));
}

ssize_t SocketOperation::write(int sockfd, const void *buf, size_t count)
{
     return TEMP_FAILURE_RETRY(::write(sockfd, buf, count));
}

void SocketOperation::getAddrAnyIpv4(struct sockaddr_in& addrIn, uint16_t port)
{
    ::bzero(&addrIn, sizeof(struct sockaddr_in));
    addrIn.sin_family = AF_INET;
    addrIn.sin_port = htons(port);
    addrIn.sin_addr.s_addr = Ipv4AddrAny;
}

bool SocketOperation::toAddrIpv4(const std::string& addrIp, struct sockaddr_in& addrIn)
{
    auto pos = addrIp.find(':');
    if(pos == addrIp.npos)
    {
        return false;
    }
    std::string strs[2];
    strs[0] = addrIp.substr(0, pos);
    strs[1] = addrIp.substr(pos+1);

    uint16_t port;
    if(!stringToInt<uint16_t>(strs[1],port))
    {
        return false;
    }
    return toAddrIpv4(strs[0],port,addrIn);
}

bool SocketOperation::toAddrIpv4(const std::string& addr, uint16_t port, struct sockaddr_in& addrIn)
{
    std::vector<std::string> ip;
    size_t first = 0;
    while(true)
    {
        auto last = addr.find('.', first);
        if(last == addr.npos)
        {
            ip.push_back(addr.substr(first,addr.size() - first));
            break;
        }
        ip.push_back(addr.substr(first, last- first));
        first = last + 1;
    }

    if(ip.size() != 4 )
    {
        return false;
    }

    uint16_t addrArray[4];
    for(int i = 0; i < 4; i++)
    {
        if(!stringToInt<uint16_t > (ip[i],addrArray[i]))
        {
            return false;
        }
        if(addrArray[i] > 255)
        {
            return false;
        }
    }

    uint32_t addr32(0);
    for(int i = 0; i < 4; i++)
    {
        addr32 <<= 8;
        addr32 |= addrArray[i];
    }

    ::bzero(&addrIn, sizeof(addrIn));
    addrIn.sin_family = AF_INET;
    addrIn.sin_port = htons(port);
    addrIn.sin_addr.s_addr = addr32;
    return true;
}

bool SocketOperation::toAddrIpv4(uint16_t port, struct sockaddr_in& addrIn)
{
    ::bzero(&addrIn, sizeof(addrIn));
    addrIn.sin_family = AF_INET;
    addrIn.sin_port = htons(port);
    addrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    return true;
}

std::string SocketOperation::ipToString(const struct sockaddr_in addr)
{
    std::stringstream stream;
    uint8_t* addrArray = (uint8_t*)&addr.sin_addr.s_addr;
    for(int i = 0; i < 4; i++)
    {
        stream << (uint16_t)addrArray[i];
        if(i!=3)
        {
            stream << ".";
        }
    }

    stream << ":" << (((addr.sin_port << 8)&0x00ffff) | (addr.sin_port>>8));
    return stream.str();
}

std::string SocketOperation::toString(const struct sockaddr_in addr)
{
    std::string addrPort;
    addrPort = ipToString(addr);
    return addrPort;
}

template<typename T>
bool SocketOperation::stringToInt(const std::string& str, T& num) {
    try {
        num = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

}
