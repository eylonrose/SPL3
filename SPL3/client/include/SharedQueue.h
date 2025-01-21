#pragma once

#ifndef SHAREDQUEUE_H
#define SHAREDQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

class SharedQueue {
private:
    std::queue<std::string> queue;
    std::mutex mtx;
    std::condition_variable cv;

public:
    SharedQueue();
    void push(const std::string& data);
    std::string pop();
};

#endif // SHAREDQUEUE_H