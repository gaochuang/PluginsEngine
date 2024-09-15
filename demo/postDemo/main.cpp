#include <iostream>
#include <eventLoop.hpp>
#include <functional>
#include <thread>
#include <chrono>

#include "engine.hpp"
#include "callbackQueueService.hpp"
#include "fdMonitor.hpp"

using namespace std;
using namespace reactorFramework;

void postCallbackEverySecond(CallbackQueueService& service) {
    while (true) {
        service.post([]() {
            cout << "postCallback" << endl;
        });
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {

    Engine engine;

    std::thread workerThread(postCallbackEverySecond, std::ref(engine.getEventCallbackQueue()));
    workerThread.detach();

    engine.run();

    return 0;
}
