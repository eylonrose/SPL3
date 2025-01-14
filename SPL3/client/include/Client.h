#include <boost/asio.hpp>
#include <iostream>

class Client {
private:
    std::string host;
    int port;
    bool connected;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;
    std::mutex mutex; 
    std::thread receiveThread;

public:
    Client(const std::string& host, int port);
    ~Client();

    bool connect(); 
    void disconnect(); 
    void send(const std::string& message);
    std::string receive();
    void run(); 
};