#include <iostream>
#include <eventLoop.hpp>
#include <functional>
#include <thread>
#include <chrono>

using namespace std;
using namespace reactorFramework;

void postCallbackEverySecond(EventLoop& loop) {
    while (true) {
        loop.postCallback([]() {
            cout << "postCallback" << endl;
        });
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    EventLoop loop;

    std::thread workerThread(postCallbackEverySecond, std::ref(loop));
    workerThread.detach();

    loop.run();

    return 0;
}
