#ifndef SOCKET_HPP_
#define SOCKET_HPP_

namespace reactorFramework
{

class Socket
{

public:
    Socket();
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(const Socket&&) = delete;
    Socket& operator=(const Socket&&) = delete;

    

};


}



#endif
