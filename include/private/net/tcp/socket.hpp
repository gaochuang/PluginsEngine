#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include "net/tcp/socketAddr.hpp"

namespace commonApi
{

class Socket
{
public:
    Socket(int fd);
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(const Socket&&) = delete;
    Socket& operator=(const Socket&&) = delete;

    void bind(SocketAddr& addr);
    int accept(SocketAddr& addr) const;
    void listen();
    int getSocketFd() const;
    void setTcpNoDelay(bool enable);
    int shutdownWrite();
private:
    int socketFd;
};


}

#endif
