#ifndef TCP_SERVER_HPP_
#define TCP_SERVER_HPP_

#include "eventLoop.hpp"
#include "socketAddr.hpp"
#include "tcpConnect.hpp"
#include "eventThreadPool.hpp"
#include "tcpAccept.hpp"

#include <map>
#include <atomic>

namespace reactorFramework
{

class TcpServer
{
public:
    explicit TcpServer(EventLoop* loop, SocketAddr& addr);

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;
    TcpServer(const TcpServer&&) = delete;
    TcpServer& operator=(const TcpServer&&) = delete;

    virtual ~TcpServer() = default;

    void start();

    // 纯虚函数，由派生类实现
    virtual void connectCallback(std::shared_ptr<TcpConnect> tcpConnect) = 0;
    virtual void messageCallback(std::shared_ptr<TcpConnect>, Buffer&) = 0;
    virtual void writeCompleteCallback(std::shared_ptr<TcpConnect> tcpConnect) = 0;
    virtual void connectCloseCallback(std::shared_ptr<TcpConnect>) = 0;

   //管理连接
    void addConnect(const std::string& name, std::shared_ptr<TcpConnect> connect);
    void addConnect(const std::string& name, TcpConnect* connect);
    bool haveConnect(const std::string& name) const;
    void removeConnect(const std::string& name);
    long getConnectCount() const;

    // 数据写入
    void write(TcpConnect& connect, void* data, uint32_t length);
    void write(const std::string& name, void* data, uint32_t length);
    void write(std::shared_ptr<TcpConnect> connect, void* data, uint32_t length);
    void write(const std::string& name, const std::string& data);

    // 设置线程池大小
    void setThreadPoolSize(uint16_t num);
private:
    // 新连接处理
    void newConnected(int sockfd, const SocketAddr& addr);
    // 关闭连接事件处理
    void connectCloseEvent(std::shared_ptr<TcpConnect> connect);

    EventLoop* eventLoop;
    SocketAddr tcpAddr;
    std::shared_ptr<TcpAccept> tcpAccept;
    std::map<std::string, std::shared_ptr<TcpConnect>> connectPool; //连接池
    std::shared_ptr<EventThreadPool> threadPool;
    std::atomic<bool> isStart;
};

}

#endif
