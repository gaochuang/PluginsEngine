#include "net/http/httpResponse.hpp"

namespace commonApi
{

HttpResponse::HttpResponse(bool close):statusCode(HttpStatusCode::Unknown),closeConnection(close)
{

}

void HttpResponse::setStatusCode(HttpStatusCode code)
{
    statusCode = code;
}

void HttpResponse::setStatusMessage(const std::string& message)
{
    statusMessage = message;
}

void HttpResponse::setCloseConnection(bool on)
{
    closeConnection = on;
}

bool HttpResponse::getCloseConnection() const
{
    return closeConnection;
}

void HttpResponse::setContentType(const std::string& contentType)
{
    addHeader("Content-Type", contentType);
}

void HttpResponse::addHeader(const std::string& key, const std::string& value)
{
    headers[key] = value;
}

void HttpResponse::setBody(const std::string& body)
{
    this->body = body;
}

void HttpResponse::appendToBuffer(Buffer* output) const
{
    char buffer[64] = {0};
    auto len = snprintf(buffer, sizeof(buffer), "HTTP/1.1 %d ", static_cast<int>(statusCode));
    output->append(buffer, len);
    output->append(statusMessage);
    output->append("\r\n");

    if(closeConnection) 
    {
        output->append("Connection: close\r\n");
    } else 
    {
        len = snprintf(buffer, sizeof(buffer), "Content-Length: %zu\r\n", body.size());
        output->append(buffer, len);
        output->append("Connection: Keep-Alive\r\n");
    }

    for (const auto& header : headers) 
    {
        output->append(header.first);
        output->append(": ");
        output->append(header.second);
        output->append("\r\n");
    }

    output->append("\r\n");
    output->append(body);
}

}