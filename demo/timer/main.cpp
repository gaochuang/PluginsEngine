#include <iostream>
#include <eventLoop.hpp>
#include <functional>
#include <ctime>
#include <iomanip>

using namespace std;

using namespace reactorFramework;

void logWithTimestamp(const std::string& message) {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    std::cout << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << " - " << message << std::endl;
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
};

int main()
{
    EventLoop loop;

    loop.runOnceAfter(std::bind(&TimerTest::callback1),1000);

    loop.runOnceAfter(std::bind(&TimerTest::callback2),3000);

    loop.runEveryInterval(std::bind(&TimerTest::callback1), 1000);

    loop.runEveryInterval(std::bind(&TimerTest::callback2), 2000);

    loop.runEveryInterval(std::bind(&TimerTest::callback3), 3000);

   

    loop.run();
}
