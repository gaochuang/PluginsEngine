#ifndef HTTP_CONTEXT_HPP_
#define HTTP_CONTEXT_HPP_

#include "httpRequest.hpp"

namespace reactorFramework
{

class Buffer;

class HttpContext
{
public:
    enum class HttpRequestParseState
    {
        ExpectRequestLine,
        ExpectHeaders,
        ExpectBody,
        GotAll,
    };

    HttpContext();
    ~HttpContext() = default;

    bool parseRequest(Buffer* buf, std::string receiveTime);
    bool getAll() const;
    void reset();
    const HttpRequest& getRequest() const;

private:
    bool processRequestLine(const char* begin, const char* end);
    HttpRequestParseState state;
    HttpRequest request;
};

}


#endif
