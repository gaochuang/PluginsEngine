#ifndef TEST_HTTP_SERVER_HPP
#define TEST_HTTP_SERVER_HPP

#include "httpServer.hpp"

class TestHttpServer : public commonApi::HttpServer
{
public:
    TestHttpServer(commonApi::EventLoop* loop, commonApi::SocketAddr& addr);
    virtual void httpCallback(const commonApi::HttpRequest&, commonApi::HttpResponse*);

};

#endif

