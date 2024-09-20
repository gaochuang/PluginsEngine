#include "net/http/httpContext.hpp"
#include "net/tcp/buffer.hpp"

#include <iostream>
#include <algorithm>

namespace commonApi
{

HttpContext::HttpContext():state(HttpRequestParseState::ExpectRequestLine)
{

}

bool HttpContext::processRequestLine(const char* begin, const char* end)
{
    bool succeed(false);
    const char* start = begin;
    //在请求行中查找第一个空格字符的位置，表示方法名和路径之间的间隔:METHOD PATH HTTP/VERSION
    const char* space = std::find(start, end, ' ');
    if(space != end && request.SetMethod(start, space))
    {
       start = space + 1;
       space = std::find(start, end, ' ');
       if(space != end)
       {
           const char* question = std::find(start, space, '?'); //GET /search?q=openai&lang=en HTTP/1.1
           //检查是否存在查询参数
           if(question != space)
           { 
               request.setPath(start, question); //设置路径
               request.setQuery(question, space); //将查询参数从问号开始到空格结束（question 到 space）设置为查询参数
           }else
           {
               request.setPath(start, space);
           }

            start = space+1;
            succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
            if (succeed)
            {
                if (*(end-1) == '1')
                {
                    request.setVersion(HttpRequest::Version::HTTP11);
                }
                else if (*(end-1) == '0')
                {
                    request.setVersion(HttpRequest::Version::HTTP10);
                }
                else
                {
                    succeed = false;
                }
            }
       }
    }

    return succeed;
}

bool HttpContext::parseRequest(Buffer* buffer, std::string receiveTime)
{

    bool ok = true;
    bool hasMore = true;
    while (hasMore)
    {
        if (state == HttpRequestParseState::ExpectRequestLine)
        {
            std::cout << "ExpectRequestLine" << std::endl;
            const char* crlf = buffer->findCRLF();
            if (crlf)
            {
                ok = processRequestLine((const char*)(buffer->readIndexPtr()), crlf);
                if (ok)
                {
                    request.setReceiveTime(receiveTime);
                    buffer->retrieveUntil(crlf + 2);
                    state = HttpRequestParseState::ExpectHeaders;
                }
                else
                {
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if (state == HttpRequestParseState::ExpectHeaders)
        {
            std::cout << "ExpectHeaders" << std::endl;
            const char* crlf = buffer->findCRLF();
            if (crlf)
            {
                const char* colon = std::find((const char*)buffer->readIndexPtr(), crlf, ':');
                if (colon != crlf)
                {
                    request.addHeader((const char*)buffer->readIndexPtr(), colon, crlf);
                }
                else
                {
                    state = HttpRequestParseState::GotAll;
                    hasMore = false;
                }
                buffer->retrieveUntil(crlf + 2);
            }
            else
            {
                hasMore = false;
            }
        }
        else if (state == HttpRequestParseState::ExpectBody)
        {

        }
    }
    return ok;
}

bool HttpContext::getAll() const
{
    return state == HttpRequestParseState::GotAll;
}

void HttpContext::reset()
{
    state = HttpRequestParseState::ExpectRequestLine;
    HttpRequest dummy;
    request.swap(dummy);
}

const HttpRequest& HttpContext::getRequest() const
{
    return request;
}

}