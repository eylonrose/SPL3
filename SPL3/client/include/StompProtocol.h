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
    std::map<std::string, int>* subs;
    int sub_id;
    int receipt_id;
    bool loggedIn;
    std::map<std::string, std::vector<Message>> messages;
    std::string name;

public:
    StompProtocol();
    virtual ~StompProtocol();
    std::string process(std::string message);
    bool processServer(std::string frame);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string getDate(int date);
    StompProtocol(const StompProtocol& other) = default;
    StompProtocol& operator=(const StompProtocol& other) = default;
    bool getLoggedIn() const;
    void summary(std::string frame);
};
