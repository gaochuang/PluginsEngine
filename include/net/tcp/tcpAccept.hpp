#ifndef TCP_ACCEPT_HPP_
#define TCP_ACCEPT_HPP_

#include <functional>
#include "eventLoop.hpp"
#include "socket.hpp"

namespace reactorFramework
{

using  connectCallBack = std::function<void (int sockfd,const SocketAddr&)> ;

class TcpAccept
{
public:
    TcpAccept(EventLoop* loop, SocketAddr addr);

    TcpAccept(const TcpAccept&) = delete;
    TcpAccept& operator=(const TcpAccept&) = delete;
    TcpAccept(const TcpAccept&&) = delete;
    TcpAccept& operator=(const TcpAccept&&) = delete;

    ~TcpAccept();

    void listen();
    bool isListen();
    void setConnectCallback(const connectCallBack callback);
private:
    EventLoop* eventLoop;
    std::shared_ptr<Socket> socket;
    std::shared_ptr<Event> event;
    bool listening;

    void acceptHandle();
    connectCallBack callBack;
};  

} 

#endif
