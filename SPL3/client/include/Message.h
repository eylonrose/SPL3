#pragma once

#include <string>
class Message {
private:
    bool active;
    bool forces;
    std::string name;
    std::string content;
    std::string city;
    std::string date_time;
    std::string event;
    int time;
public:
    Message(std::string name, std::string content,std::string city, std::string date,std::string event, bool forces, bool active, int time);
    bool isActive() const;
    bool isForces() const;
    std::string getName() const;
    std::string getContent() const;
    std::string getCity() const;
    std::string getDateTime() const;
    std::string getEvent() const;
    int getTime() const;
};