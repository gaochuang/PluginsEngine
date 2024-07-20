#ifndef SOCKET_ADDR_HPP
#define SOCKET_ADDR_HPP

#include <string>
#include <netinet/in.h>

namespace reactorFramework
{

class SocketAddr
{
public:
    SocketAddr();
    explicit SocketAddr(bool isAny, int port = 0);
    SocketAddr(struct sockaddr_in addr);
    SocketAddr(const std::string& addrPort);
    SocketAddr(const std::string& addr,uint16_t port);
    SocketAddr(uint16_t port);


    ~SocketAddr();

    void setAddr(struct sockaddr_in addr);
    struct sockaddr_in getAddr() const;
    struct sockaddr_in* getAddrPtr();
    bool isValid();
    std::string toString() const ;
private:
    bool valid;
    struct sockaddr_in sockAddr;
};

}

#endif
