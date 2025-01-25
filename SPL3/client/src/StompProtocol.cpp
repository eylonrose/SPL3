#include "StompProtocol.h"
#include "event.h"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <iostream>


StompProtocol::StompProtocol(): subs(nullptr),sub_id(0),receipt_id(0),loggedIn(false),messages(),name(""){
    subs = new std::map<std::string, int>();
    sub_id = 0;
    receipt_id = 0;
    loggedIn = false;
    messages = std::map<std::string, std::vector<Message>>();
}

StompProtocol::~StompProtocol()
{
}

std::string StompProtocol::process(std::string message)
{
    std::vector<std::string> tokens = split(message, ' ');
    if(tokens.size() == 0)
    {
        std::cout << "Please input a command" << std::endl;
        return "";
    }
    std::string command = tokens[0];
    if(command == "login")
    {
        if(loggedIn)
        {
            std::cout << "You are already logged in" << std::endl;
            return "";
        }
        if(tokens.size() != 4)
        {
            std::cout << "Invalid number of arguments, Should be login {port} {username} {password}" << std::endl;
            return "";
        }
        std::vector<std::string> connect = split(tokens[1], ':');
        if(connect.size() != 2)
        {
            std::cout << "Invalid port format, Should be port {host}:{port}" << std::endl;
            return "";
        }  
            std::string login = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:" + tokens[2] + "\npasscode:" + tokens[3] + "\n\n";
            loggedIn = true;
            name = tokens[2];
            return login;
    }
    else if(!loggedIn)
    {
        std::cout << "Please login first" << std::endl;
        return "";
    }
    else if(command == "logout")
    {
        if(tokens.size() != 1)
        {
            std::cout << "Invalid number of arguments, Should be logout" << std::endl;
            return "";
        }
        std::string logout = "DISCONNECT\nreceipt:"+std::to_string(receipt_id)+"\n\n";
        std::cout << "Logging out" << std::endl;
        receipt_id++;
        loggedIn = false;
        subs->clear();
        return logout;
    }
    else if(command == "join")
    {
        if(tokens.size() != 2)
        {
            std::cout << "Invalid number of arguments, Should be join {channel_name}" << std::endl;
            return "";
        }
        std::string join = "SUBSCRIBE\ndestination:" + tokens[1] + "\nid:"+std::to_string(sub_id)+"\nreceipt:"+std::to_string(receipt_id)+"\n\n";
        subs->insert(std::pair<std::string, int>(tokens[1], sub_id));
        sub_id++;
        receipt_id++;
        std::cout << "Joined channel " << tokens[1] << std::endl;
        return join;
    }
    else if(command == "exit")
    {
        if(tokens.size() != 2)
        {
            std::cout << "Invalid number of arguments, Should be exit {channel_name}" << std::endl;
            return "";
        }
        if(subs->find(tokens[1]) == subs->end())
        {
            std::cout << "You are not subscribed to this channel" << std::endl;
            return "";
        }
        std::string exit = "UNSUBSCRIBE\nid:"+std::to_string(subs->at(tokens[1]))+"\nreceipt:"+std::to_string(receipt_id)+"\n\n";
        subs->erase(tokens[1]);
        receipt_id++;
        std::cout << "Exited channel " << tokens[1] << std::endl;
        return exit;
    }
    else if(command == "report")
    {
        std::string report;
        if(tokens.size() != 2)
        {
            std::cout << "Invalid number of arguments, Should be report {file_path}" << std::endl;
            return "";
        }
        names_and_events names_and_events = parseEventsFile(tokens[1]);
        for(Event event : names_and_events.events)
        {
            std::cout << "Sending report for event " << event.get_name() << std::endl;
            std::string event_name = event.get_name();
            std::string event_city = event.get_city();
            int event_date_time = event.get_date_time();
            std::string event_description = event.get_description();
            std::map<std::string, std::string> general_information = event.get_general_information();
            std::string send = "SEND\ndestination:" + names_and_events.channel_name + "\n" + "user:" + name +"\n"+ "city:" + event_city + "\n"+ "event name:" + event_name + "\n" +  "date time:" + std::to_string(event_date_time) + "\n" +"general information:\n";
            for(std::pair<std::string, std::string> info : general_information)
            {
                send += info.first + ":" + info.second + "\n";
            }
            send += "description:" + event_description + "\n\n";
            report += send;
            report += "next";
        
        }
        return report;
    }
    else
    {
        std::cout << "Invalid command" << std::endl;
    }
    return "";
}

bool StompProtocol::processServer(std::string frame)
{   
    if(!loggedIn)
    {
        return false;
    }
    /*while(connectionHandler->getLine(frame))
    {
        std::cout << frame << std::endl;
        if (frame.size() >= 2 && frame[frame.size() - 2] == '\n' && frame[frame.size() - 1] == '\n')
        {
            break;
        }
    }*/
    
        std::vector<std::string> tokens = split(frame, '\n');
        if(tokens[0] == "MESSAGE")
        {
            std::string channel = split(tokens[1],':')[1];
            std::string name = split(tokens[2],':')[1];
            std::string city = split(tokens[3],':')[1];
            std::string date_time = split(tokens[5],':')[1];
            int time = std::stoi(date_time);
            date_time = getDate(time);
            std::string event = split(tokens[4],':')[1];
            bool active = split(tokens[6],':')[1] == "true";
            bool forces = split(tokens[7],':')[1] == "true";
            std::string content = split(tokens[8],':')[1];
            content = content.substr(0, 27) + "...";
            Message message(name, content,city,date_time,event, forces, active, time);
            if(messages.find(channel) == messages.end())
            {
                messages.insert(std::pair<std::string, std::vector<Message>>(channel, std::vector<Message>()));
            }
            messages[channel].push_back(message);
        }
        if(tokens[0] == "ERROR")
        {
            std::string error = split(tokens[1],':')[1];
            if(error == "Wrong password")
            {
                loggedIn = false;
            }
            if(error == "User already logged in")
            {
                std::cout << "User already logged in from another client" << std::endl;
                loggedIn = false;
            }
            for(int i = 0; i < static_cast<int>(tokens.size()); i++)
            {
                std::cout << tokens[i] << std::endl;
            }
            loggedIn = false;
        }
        if(tokens[0] == "CONNECTED")
        {
            std::cout << "Login successful" << std::endl;
        }
        return true;
}


std::vector<std::string> StompProtocol::split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::string StompProtocol::getDate(int date)
{
    time_t epochTime = static_cast<time_t>(date);
// Convert epoch to tm struct
    std::tm* tm = std::localtime(&epochTime);

    // Format the tm struct into a string
    std::ostringstream oss;
    oss << std::put_time(tm, "%d/%m/%y %H:%M");  // Format: DD/MM/YY HH:MM

    return oss.str();
}

bool StompProtocol::getLoggedIn() const
{
    return loggedIn;
}

void StompProtocol::summary(std::string frame)
{
    std::vector<std::string> tokens = split(frame, ' ');
    if(tokens.size() != 4)
        {
            std::cout << "Invalid number of arguments, Should be summary {channel_name} {user} {file}" << std::endl;
            return;
        }

        int forces = 0;
        int active = 0;
        int total = 0;
        std::vector<Message> channel_messages = messages[tokens[1]];
        std::vector<Message> user_messages;
        for(Message message : channel_messages)
        {
            if(message.getName() == tokens[2])
            {
                total++;
                if(message.isForces())
                {
                    forces++;
                }
                if(message.isActive())
                {
                    active++;
                }
                user_messages.push_back(message);
            }
        }

        std::sort(user_messages.begin(), user_messages.end(), [](const Message& a, const Message& b) {
        if (a.getTime() == b.getTime()) {
            return a.getEvent() < b.getEvent(); // Lexicographic comparison of events
        }
        return a.getTime() < b.getTime(); // Compare by time
    });

        std::ofstream outFile(tokens[3], std::ios::trunc);
        outFile << "Channel: " << tokens[1] << std::endl;
        outFile << "Stats:" << std::endl;
        outFile << "Total:" << total << std::endl;
        outFile << "Active:" << active << std::endl;
        outFile << "Forces arrival at scene :" << forces << std::endl;
        outFile << std::endl;
        outFile << "Event Reports :" << std::endl;
        int count = 1;
        for(Message message : user_messages)
        {
                outFile << std::endl;
                outFile << "Report_"<< count<<":" << std::endl;
                outFile << "City:" << message.getCity() << std::endl;
                outFile << "Date time:" << message.getDateTime() << std::endl;
                outFile << "Event name:" << message.getEvent() << std::endl;
                outFile << "Summary:" << message.getContent() << std::endl;
                count++;
        }
        outFile.close();    


}
