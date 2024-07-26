#ifndef HTTP_REQUEST_HPP_
#define HTTP_REQUEST_HPP_

#include <map>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <unordered_map>

namespace reactorFramework
{

class HttpRequest
{
public:
    enum class Method
    {
        Invalid,
        Get,
        Post,
        Head,
        Put,
        Delete
    };
    enum class Version
    {
        Unkown,
        HTTP10,
        HTTP11
    };

    static const std::unordered_map<std::string, Method> methodMap;

    HttpRequest();
    ~HttpRequest() = default;

    void setVersion(Version v);
    Version getVersion() const;

    bool SetMethod(const char* start, const char* end);
    Method getMethod() const;
    std::string methodToSting() const;

    void setPath(const char* start, const char* end);
    const std::string& getPath() const;

    void setQuery(const char* start, const char* end);
    const std::string& getQuery();

    void setReceiveTime(std::string& t);
    std::string getReveiveTimer() const;

    void addHeader(const char* start, const char* end, const char* colon);
    std::string getHead(const std::string& field) const;
    const std::map<std::string, std::string>& getHeaders() const;
    void swap(HttpRequest& that);

private:
   Method method;
   Version version;
   std::string path;
   std::string query;
   std::string receiveTime;
   std::map<std::string, std::string> headers;
};


}




#endif
