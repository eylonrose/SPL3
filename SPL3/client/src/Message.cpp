#include "Message.h"



Message::Message(std::string name, std::string content, std::string city, std::string date, std::string event, bool forces, bool active): active(active), forces(forces), name(name), content(content), city(city), date_time(date), event(event)
{
}

bool Message::isActive()
{
    return active;
}

bool Message::isForces()
{
    return forces;
}

std::string Message::getName()
{
    return name;
}

std::string Message::getContent()
{
    return content;
}

std::string Message::getCity()
{
    return city;
}

std::string Message::getDateTime()
{
    return date_time;
}

std::string Message::getEvent()
{
    return event;
}

void Message::setActive(bool active)
{
    this->active = active;
}

void Message::setForces(bool forces)
{
    this->forces = forces;
}

void Message::setName(std::string name)
{
    this->name = name;
}

void Message::setContent(std::string content)
{
    this->content = content;
}
