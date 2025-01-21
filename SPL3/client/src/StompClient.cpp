#include <iostream>
#include <sstream>
#include "keyboardInput.h"
#include <thread>
#include "StompProtocol.h"
#include "SharedQueue.h"


int main(int argc, char *argv[]) {
    SharedQueue* sharedQueue = new SharedQueue();
    KeyboardInput* keyboardInput = new KeyboardInput(sharedQueue);
    StompProtocol* stompProtocol = new StompProtocol(sharedQueue);
    std::thread keyboardThread(*keyboardInput);
    std::thread stompThread(*stompProtocol);
    keyboardThread.join();
    stompThread.join();
    return 0;
}
