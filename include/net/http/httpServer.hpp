#ifndef HTTP_SERVER_HPP_
#define HTTP_SERVER_HPP_

#include "tcpServer.hpp"
#include "httpResponse.hpp"
#include "httpRequest.hpp"

namespace reactorFramework 
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

