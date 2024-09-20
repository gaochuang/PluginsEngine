#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "net/tcp/buffer.hpp"

#include <unordered_map>
#include <string>

namespace commonApi
{
class HttpResponse
{
public:
    enum class HttpStatusCode
    {
        Unknown = 0,
        Ok = 200,
        MovedPermanently = 301,
        BadRequest = 400,
        NotFound = 404,
    };

    explicit HttpResponse(bool close);
    ~HttpResponse() = default;

    void setStatusCode(HttpStatusCode code);
    void setStatusMessage(const std::string& message);
    void setCloseConnection(bool on);
    bool getCloseConnection() const;
    void setContentType(const std::string& contentType);
    void addHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    void appendToBuffer(Buffer* output) const;
private:
    std::unordered_map<std::string, std::string> headers;
    HttpStatusCode statusCode;
    std::string statusMessage;
    bool closeConnection;
    std::string body;
};

}
#endif