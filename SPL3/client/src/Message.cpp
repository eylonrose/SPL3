#include "Message.h"



Message::Message(std::string name, std::string content, std::string city, std::string date, std::string event, bool forces, bool active, int time): active(active), forces(forces), name(name), content(content), city(city), date_time(date), event(event), time(time)
{
}

bool Message::isActive() const
{
    return active;
}

bool Message::isForces() const
{
    return forces;
}

std::string Message::getName() const
{
    return name;
}

std::string Message::getContent() const
{
    return content;
}

std::string Message::getCity() const
{
    return city;
}

std::string Message::getDateTime() const
{
    return date_time;
}

std::string Message::getEvent() const
{
    return event;
}

int Message::getTime() const
{
    return time;
}

