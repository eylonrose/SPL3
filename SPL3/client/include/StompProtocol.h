#pragma once

#include "../include/ConnectionHandler.h"
#include <string>
#include <iostream>
#include "SharedQueue.h"
#include "Message.h"
#include <map>
#include <vector>

// TODO: implement the STOMP protocol
class StompProtocol
{
private:
    ConnectionHandler* connectionHandler;
    std::map<std::string, int>* subs;
    int sub_id;
    int receipt_id;
    SharedQueue* sharedQueue;
    std::string frame;
    bool loggedIn;
    std::map<std::string, std::vector<Message>> messages;
    std::string name;

public:
    StompProtocol(SharedQueue* sharedQueue);
    virtual ~StompProtocol();
    void process();
    void processServer();
    void operator()();
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string getDate(std::string date);
    StompProtocol(const StompProtocol& other) = default;
    StompProtocol& operator=(const StompProtocol& other) = default;
};
