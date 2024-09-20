#include <iostream>
#include <eventLoop.hpp>
#include <functional>
#include <ctime>
#include <iomanip>

#include "engine.hpp"

using namespace std;
using namespace commonApi;

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
    static void callback1() { logWithTimestamp("callback 1 test"); }
    static void callback2() { logWithTimestamp("callback 2 test"); }
    static void callback3() { logWithTimestamp("callback 3 test"); }
    static void callback4() { logWithTimestamp("callback 4 test"); }
    static void runInLoop() { logWithTimestamp("run in loop"); }
};

int main()
{
    Engine engine;
    

    // 添加一些单次定时器
    for (int i = 0; i < 50000; ++i) {
        // 延迟时间从 100ms 到 5000ms
        int delay = (i % 50) * 100; 
        engine.getTimerService().runEveryInterval(std::bind(&TimerTest::callback1), delay);
    }

    // 添加一些周期性定时器
    for (int i = 0; i < 5000; ++i) {
        // 间隔时间从 100ms 到 5000ms
        int interval = (i % 50 + 1) * 100; 
        engine.getTimerService().runEveryInterval(std::bind(&TimerTest::callback2), interval);
    }

    // 启动事件循环
    engine.run();
}
