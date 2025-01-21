#pragma once

#include <string>
#include <iostream>
#include "SharedQueue.h"



class KeyboardInput {
private:
    SharedQueue* sharedQueue;
    
public:
    KeyboardInput(SharedQueue* sharedQueue);
    void operator()();
};