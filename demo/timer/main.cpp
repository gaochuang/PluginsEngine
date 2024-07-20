#include <iostream>
#include <eventLoop.hpp>
#include <functional>
#include <ctime>
#include <iomanip>

using namespace std;

using namespace reactorFramework;

void logWithTimestamp(const std::string& message) {
    // 获取当前时间
    auto now = std::time(nullptr);
    // 将时间转换为本地时间
    auto localTime = std::localtime(&now);
    
    // 手动格式化时间
    char buffer[100];
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime)) {
        std::cout << buffer << " - " << message << std::endl;
    } else {
        std::cerr << "Failed to format time" << std::endl;
    }
}

class TimerTest
{
public:
    static void callback1()
    {
        logWithTimestamp("callback 1 test");
    }
    static void callback2()
    {
        logWithTimestamp("callback 2 test");
    }
    static void callback3()
    {
        logWithTimestamp("callback 3 test");
    }
    static void callback4()
    {
        logWithTimestamp("callback 4 test");
    }

    static void runInLoop()
    {
        logWithTimestamp("run in loop");
    }
};

int main()
{
    EventLoop loop;

    loop.runOnceAfter(std::bind(&TimerTest::callback1),1000);

    loop.runOnceAfter(std::bind(&TimerTest::callback2),3000);

    loop.runEveryInterval(std::bind(&TimerTest::callback1), 1000);

    loop.runEveryInterval(std::bind(&TimerTest::callback2), 2000);

    loop.runEveryInterval(std::bind(&TimerTest::callback3), 3000);

    loop.runInLoop(std::bind(&TimerTest::runInLoop));

   

    loop.run();
}
