#ifndef BUFFER_HPP_
#define BUFFER_HPP_

/**
这个 Buffer 类是一个用于管理内存缓冲区的类，主要用于在 I/O 操作中存储和处理数据。
封装了数据缓冲区的操作，使得在读取、写入和处理数据时更加高效和方便。
*/

#include <string>
#include <vector>
#include <cstdint>

namespace reactorFramework
{

class Buffer
{
public:
    static constexpr uint32_t InitSize = 1024;
    explicit Buffer(uint32_t size = InitSize);

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    ~Buffer() = default;

    bool isReadable() const;
    bool isEmpty() const;

    int readFromIO(int fd, int& errorNo);
    uint32_t writableBytes() const;
    uint32_t readableBytes() const;
    void append(const char* data, std::size_t len);
    void append(const std::string& str);
    void ensureWritableBytes(std::size_t len);
    const uint8_t* readIndexPtr() const;
    void resetIndex();
    void clearReadIndex(uint32_t len);
    int readAllAsString(std::string &readBuf);
    int readAsString(uint32_t len, std::string &readBuf);
    int readAllAsByte(std::vector<uint8_t>& data);
    int readBytes(std::vector<uint8_t>& data, uint32_t len);
    void swap(Buffer& rhs);
    const char* findCRLF() const;
    void retrieveUntil(const char* end);

private:
    uint32_t readIndex;
    uint32_t writeIndex;
    std::vector<uint8_t> buffer; //实际的存储数据的内存，使用std::vector<uint8_t>管理

    const uint8_t* bufferPtr() const;
};


}
#endif
