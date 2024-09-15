#include "httpContext.hpp"
#include "httpContext.hpp"
#include "httpServer.hpp"
#include "iostream"

namespace reactorFramework
{

HttpServer::HttpServer(EventLoop* loop,SocketAddr& addr):TcpServer(loop,addr)
{

}

HttpServer::~HttpServer()
{

}

void HttpServer::connectCallback(std::shared_ptr<TcpConnect> tcpConnect)
{

}

void HttpServer::messageCallback(std::shared_ptr<TcpConnect> tcpConnect, Buffer& buffer)
{
    HttpContext context;
    if (!context.parseRequest(&buffer, Timer::getNowTimeDate()))
    {
        tcpConnect->write("HTTP/1.1 400 Bad Request\r\n\r\n");
        tcpConnect->shutdownWrite();
    }

    if (context.getAll())
    {
        onRequest(tcpConnect, context.getRequest());
        context.reset();
    }
}

void HttpServer::writeCompleteCallback(std::shared_ptr<TcpConnect> tcpConnect)
{

}

void HttpServer::connectCloseCallback(std::shared_ptr<TcpConnect> connect)
{

}

void HttpServer::httpCallback(const HttpRequest&, HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::HttpStatusCode::NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

void HttpServer::onRequest( std::shared_ptr<TcpConnect> conn, const HttpRequest& req)
{
    const std::string& connection = req.getHead("Connection");
    bool close = connection == "close" ||(req.getVersion() == HttpRequest::Version::HTTP10 && connection != "Keep-Alive");
    HttpResponse response(close);

    httpCallback(req, &response);
    Buffer buf;
    response.appendToBuffer(&buf);
    std::string str;
    buf.readAllAsString(str);
    conn->write(str);
    if (response.getCloseConnection())
    {
        conn->shutdownWrite();
    }
}

}
