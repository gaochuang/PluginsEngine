#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "net/tcp/tcpServer.hpp"
#include "net/http/httpResponse.hpp"
#include "net/http/httpRequest.hpp"

namespace commonApi 
{

class HttpServer : public TcpServer
{
public:
    HttpServer(EventLoop* loop, SocketAddr& addr);
    virtual ~HttpServer();

    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;
    HttpServer(const HttpServer&&) = delete;
    HttpServer& operator=(const HttpServer&&) = delete;

private:
    virtual void connectCallback(std::shared_ptr<TcpConnect> tcpConnect);
    virtual void messageCallback(std::shared_ptr<TcpConnect>, Buffer&);
    virtual void writeCompleteCallback(std::shared_ptr<TcpConnect> tcpConnect);
    virtual void connectCloseCallback(std::shared_ptr<TcpConnect>);

    virtual void httpCallback(const HttpRequest&, HttpResponse*);

    void onRequest(std::shared_ptr<TcpConnect> conn, const HttpRequest& req);
};

} 

#endif

