#ifndef TCP_CONNECT_HPP
#define TCP_CONNECT_HPP

#include "eventLoop.hpp"
#include "socket.hpp"
#include "buffer.hpp"

#include <memory>
#include <functional>

namespace reactorFramework
{

class TcpConnect : public std::enable_shared_from_this<TcpConnect>
{
public:
    using oneMessageCallback = std::function<void (std::shared_ptr<TcpConnect>, Buffer&)>;
    using oneCloseCallback = std::function<void (std::shared_ptr<TcpConnect>)>;
    using oneWriteCompleteCallback = std::function<void (std::shared_ptr<TcpConnect>)>;
    
    TcpConnect(EventLoop* loop, struct sockaddr_in addr, int fd);

    TcpConnect(const TcpConnect&) = delete;
    TcpConnect& operator=(const TcpConnect&) = delete;
    TcpConnect(const TcpConnect&&) = delete;
    TcpConnect& operator=(const TcpConnect&&) = delete;

    ~TcpConnect();

    void setMessageCallback(const oneMessageCallback callback);
    void setCloseCallback(const oneCloseCallback callback);
    void setWriteCompleteCallback(const oneWriteCompleteCallback callback);


    const SocketAddr& getAddr() const;
    std::string getName() const;


    void write(const char* data);
    void write(const std::string& data);
    void write(const void* data, uint32_t length);
    void writeInLoop(const void* data, uint32_t length);

    void connectedHandle();
    void setNoDelay(bool enable);
    void shutdownWrite();

private:
    void readEvent();
    void closeEvent();
    void writeEvent();
    void errorEvent();

    EventLoop* loop;
    SocketAddr socketAddr;
    std::string name;
    std::shared_ptr<Socket> socket;
    std::shared_ptr<Event> event;
    Buffer readBuf;
    Buffer writeBuf;

    oneMessageCallback messageCallback;
    oneCloseCallback closeCallback;
    oneWriteCompleteCallback writeCompleteCallback;

    enum ConnectState
    {
        Disconnected,
        Connecting,
        Connected,
        Disconnecting
    };

    ConnectState state;

    TcpConnect& getReference();
};
}

#endif
