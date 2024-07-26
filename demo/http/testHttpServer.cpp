#include "testHttpServer.hpp"

using namespace reactorFramework;

TestHttpServer::TestHttpServer(EventLoop* loop, SocketAddr& addr):HttpServer(loop,addr)
{

}

void TestHttpServer::httpCallback(const HttpRequest&, HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::HttpStatusCode::Ok);
    resp->setStatusMessage("OK");
    resp->setBody("this is test message");
    resp->setCloseConnection(true);
}
