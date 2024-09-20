#include <iostream>
#include <eventLoop.hpp>
#include <functional>
#include <thread>
#include <chrono>

#include "engine.hpp"
#include "callbackQueueService.hpp"
#include "fdMonitor.hpp"
#include "comApi.hpp"

using namespace std;
using namespace commonApi;

int main() {
    
    auto api = ComAPI::create();

    api->postCallback(
        [](){
            cout << "postCallback" << endl;
        }
    );

    while(1)
    {
        api->waitAndHandleEvents();
    }

    return 0;
}
