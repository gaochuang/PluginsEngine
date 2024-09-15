#ifndef TEST_HTTP_SERVER_HPP
#define TEST_HTTP_SERVER_HPP

#include "httpServer.hpp"

class TestHttpServer : public reactorFramework::HttpServer
{
public:
    TestHttpServer(reactorFramework::EventLoop* loop, reactorFramework::SocketAddr& addr);
    virtual void httpCallback(const reactorFramework::HttpRequest&, reactorFramework::HttpResponse*);

};

#endif

