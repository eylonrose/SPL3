
#include "keyboardInput.h"
#include "SharedQueue.h"



KeyboardInput::KeyboardInput(SharedQueue* queue): sharedQueue(queue)
{
}



void KeyboardInput::operator()()
{
    while (true)
    {
        std::string input;
        std::getline(std::cin, input);
        sharedQueue->push(input);
    }
}