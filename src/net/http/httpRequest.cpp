#include "httpRequest.hpp"

namespace reactorFramework
{

const std::unordered_map<std::string, HttpRequest::Method> HttpRequest::methodMap = {
        {"GET", Method::Get},
        {"POST", Method::Post},
        {"HEAD", Method::Head},
        {"PUT", Method::Put},
        {"DELETE", Method::Delete}
    };

HttpRequest::HttpRequest():method(Method::Invalid),version(Version::Unkown)
{

}

void HttpRequest::setVersion(Version version)
{
    this->version = version;
}

HttpRequest::Version HttpRequest::getVersion() const
{
    return version;
}


bool HttpRequest::SetMethod(const char* start, const char* end)
{
    assert(method == Method::Invalid);

    std::string m(start, end);

    auto it = methodMap.find(m);
    if (it != methodMap.end()) 
    {
        method = it->second;
    } else 
    {
        method = Method::Invalid;
    }

    return method != Method::Invalid;
}

HttpRequest::Method HttpRequest::getMethod() const
{
    return method;
}

std::string HttpRequest::methodToSting() const
{
    const char* result = "UNKNOWN";
    switch(method)
    {
      case Method::Get:
            result = "GET";
            break;
      case Method::Post:
            result = "POST";
            break;
      case Method::Head:
            result = "HEAD";
            break;
      case Method::Put:
            result = "PUT";
            break;
      case Method::Delete:
            result = "DELETE";
            break;
      default:
            break;
    }
    return result;
}

void HttpRequest::setPath(const char* start, const char* end)
{
    path.assign(start, end);
}

const std::string& HttpRequest::getPath() const
{
    return path;
}

void HttpRequest::setQuery(const char* start, const char* end)
{
    query.assign(start, end);
}

const std::string& HttpRequest::getQuery()
{
    return query;
}

void HttpRequest::setReceiveTime(std::string& t)
{
    receiveTime = t;
}

std::string HttpRequest::getReveiveTimer() const
{
    return receiveTime;
}

void HttpRequest::addHeader(const char* start, const char* end, const char* colon)
{
    std::string field(start, colon);

    // 跳过冒号后的空白字符
    ++colon;
    while (colon < end && isspace(*colon))
    {
        ++colon;
    }

    // 提取字段值
    std::string value(colon, end);

    // 去除字段值末尾的空白字符
    auto it = value.find_last_not_of(" \t\r\n");
    if (it != std::string::npos) 
    {
        value.erase(it + 1);
    }

    // 添加到头部集合
    headers[field] = value;
}

std::string HttpRequest::getHead(const std::string& field) const
{
    std::string result;
    std::map<std::string, std::string>::const_iterator it = headers.find(field);
    if (it != headers.end())
    {
        result = it->second;
    }
    return result;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const
{
    return headers;
}

void HttpRequest::swap(HttpRequest& that)
{
    std::swap(method, that.method);
    std::swap(version, that.version);
    path.swap(that.path);
    query.swap(that.query);
     receiveTime.swap(that.receiveTime);
    headers.swap(that.headers);
}

}
