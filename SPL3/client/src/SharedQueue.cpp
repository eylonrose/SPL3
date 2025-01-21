#include "SharedQueue.h"

SharedQueue::SharedQueue(): queue(), mtx(), cv()
{
}

void SharedQueue::push(const std::string &data)
{
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(data);
    cv.notify_one();
}

std::string SharedQueue::pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return !queue.empty(); }); // Wait for data
    std::string data = queue.front();
    queue.pop();
    return data;
}