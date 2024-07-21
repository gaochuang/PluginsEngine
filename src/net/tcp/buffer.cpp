#include "buffer.hpp"
#include "socketOperation.hpp"

#include <errno.h>
#include <sys/uio.h>
#include <algorithm>
#include <iostream>

namespace reactorFramework
{

Buffer::Buffer(uint32_t size):readIndex(0),writeIndex(0),buffer(size)
{

}

inline const uint8_t* Buffer::bufferPtr() const
{
    return buffer.data();
}

inline bool Buffer::isReadable() const 
{
    return readIndex < writeIndex;
}

bool Buffer::isEmpty() const 
{
    return readIndex == writeIndex;
}

inline uint32_t Buffer::writableBytes() const 
{
    return buffer.size() - writeIndex;
}

uint32_t Buffer::readableBytes() const 
{
    return writeIndex - readIndex;
}

inline void Buffer::resetIndex() 
{
    readIndex = 0;
    writeIndex = 0;
}

/*
readv函数读取数据到缓冲区和额外的栈缓冲区extrabuf
如果数据超出缓冲区容量，追加到缓冲.
*/
int Buffer::readFromIO(int fd, int& errorNo)
{
    char extrabuf[65536];

    struct iovec vec[2];

    auto writable = writableBytes();

    // vec[0] 指向缓冲区的可写部分
    vec[0].iov_base = buffer.data() + writeIndex;
    vec[0].iov_len = writable;

    // vec[1] 指向额外的栈缓冲区
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    // 如果可写空间不足以容纳所有数据，使用两个 iovec；否则只使用一个
    auto iovcnt = writable < sizeof(extrabuf) ? 2 : 1;

    // 读取数据到缓冲区和额外缓冲区中
    auto n = SocketOperation::readv(fd, vec, iovcnt);
    if(n < 0)
    {
        errorNo = errno;
    }else if(static_cast<uint32_t>(n) <= writable)
    {
        //如果读取的字节数 n 小于或等于缓冲区的可写空间 writable，所有数据都可以直接写入缓冲区
        writeIndex += n;
    }else
    {
        //读取的字节数 n 大于缓冲区的可写空间 writable，部分数据需要写入额外缓冲区 extrabuf
        writeIndex = buffer.size();
       //将缓冲区的写入索引 writeIndex 设置为缓冲区的大小 buffer.size()，缓冲区已满，
       //将多余的数据（即 n - writable 字节）追加到缓冲区中。将 extrabuf 中的数据追加到缓冲区中
        append(extrabuf, n - writable);
    }

    return n;
}

/*
将数据追加到缓冲区。如果需要，扩展缓冲区的容量
*/
void Buffer::append(const char* data, std::size_t len)
{
    ensureWritableBytes(len);
    std::copy(data, data+len, buffer.begin() + writeIndex);
    writeIndex += len;
}

void Buffer::append(const std::string& str)
{
    append(str.data(), str.size());
}

/*
确保缓冲区有足够的可写空间。如果不足，扩展缓冲区 
*/
void Buffer::ensureWritableBytes(std::size_t len)
{
    if(writableBytes() < len)
    {
        buffer.resize(writeIndex + len);
    }
}

const uint8_t* Buffer::readIndexPtr() const
{
    return bufferPtr() + readIndex;
}

void Buffer::clearReadIndex(uint32_t len)
{
    if(len < readableBytes())
    {
        readIndex += len;
    }else
    {
        resetIndex();
    }
}

int Buffer::readAllAsString(std::string &readBuf)
{
    return readAsString(readableBytes(), readBuf);
}

int Buffer::readAsString(uint32_t len, std::string &readBuf)
{
    auto readable = readableBytes();
    if(len > readable)
    {
        len = readable;
    }

    readBuf.assign(reinterpret_cast<const char*>(readIndexPtr()), len);
    clearReadIndex(len);
    return len;
}
    
int Buffer::readAllAsByte(std::vector<uint8_t>& data)
{
    return readBytes(data,readableBytes());
}

int Buffer::readBytes(std::vector<uint8_t>& data, uint32_t len)
{
    auto readable = readableBytes();
    if(len > readable)
    {
        len = readable;
    }

    data.assign(readIndexPtr(), readIndexPtr()+len);
    clearReadIndex(len);
    return len;
}

void Buffer::swap(Buffer& rhs)
{
    buffer.swap(rhs.buffer);
    std::swap(readIndex, rhs.readIndex);
    std::swap(writeIndex, rhs.writeIndex);
}

const char* Buffer::findCRLF() const
{
    constexpr char kCRLF[] = "\r\n";
    const uint8_t* crlf = std::search(readIndexPtr(), buffer.data() + writeIndex, kCRLF, kCRLF + 2);
    return (crlf == buffer.data() + writeIndex) ? nullptr : reinterpret_cast<const char*>(crlf);
}

/*
更新读索引到end位置，以便标记数据已经被读取
*/
void Buffer::retrieveUntil(const char* end)
{
    const uint8_t* start = readIndexPtr();
    const uint8_t* endPtr = reinterpret_cast<const uint8_t*>(end);
    const uint8_t* bufferEnd = bufferPtr() + writeIndex;
    if (endPtr < start || endPtr > bufferEnd)
    {
        std::cerr << "retrieveUntil: End pointer out of range" << std::endl;
        return;
    }
    clearReadIndex(static_cast<uint32_t>(endPtr - start));
}

}
