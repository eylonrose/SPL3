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
public:
    Message(std::string name, std::string content,std::string city, std::string date,std::string event, bool forces, bool active);
    bool isActive();
    bool isForces();
    std::string getName();
    std::string getContent();
    std::string getCity();
    std::string getDateTime();
    std::string getEvent();
    void setActive(bool active);
    void setForces(bool forces);
    void setName(std::string name);
    void setContent(std::string content);

};