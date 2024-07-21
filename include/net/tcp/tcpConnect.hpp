#ifndef TCP_CONNECT_HPP
#define TCP_CONNECT_HPP

#include "eventLoop.hpp"
#include "socket.hpp"
#include "buffer.hpp"
#include <memory>

namespace reactorFramework
{

class TcpConnect : public std::enable_shared_from_this<TcpConnect>
{
public:
    using MessageCallback = std::function<void(std::shared_ptr<TcpConnect>, Buffer&)>;
    using CloseCallback = std::function<void(std::shared_ptr<TcpConnect>)>;
    using WriteCompleteCallback = std::function<void(std::shared_ptr<TcpConnect>)>;

    TcpConnect(EventLoop* loop, const struct sockaddr_in& addr, int fd);
    TcpConnect(const TcpConnect&) = delete;
    TcpConnect& operator=(const TcpConnect&) = delete;
    TcpConnect(const TcpConnect&&) = delete;
    TcpConnect& operator=(const TcpConnect&&) = delete;

    ~TcpConnect();

    void setMessageCallback(MessageCallback callback);
    void setCloseCallback(CloseCallback callback);
    void setWriteCompleteCallback(WriteCompleteCallback callback);


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

    MessageCallback messageCallback;
    CloseCallback closeCallback;
    WriteCompleteCallback writeCompleteCallback;

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
