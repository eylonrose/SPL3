#include "Client.h"
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

Client::Client(const std::string& host, int port)
    : host(host), port(port), connected(false), socket(io_service) {}

Client::~Client() {
    disconnect();
}

bool Client::connect() {
    try {
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(host, std::to_string(port));
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        boost::asio::connect(socket, endpoint_iterator);
        connected = true;
        std::cout << "Connected to server" << std::endl;
        return true;
    } catch (std::exception& e) {
        std::cerr << "Failed to connect: " << e.what() << std::endl;
        return false;
    }
} 
    void Client::disconnect() {
        if (receiveThread.joinable()) {
        receiveThread.join();
    }
    if (connected) {
        boost::system::error_code ec;
        socket.close(ec);
        if (!ec) {
            std::cout << "Disconnected from server" << std::endl;
        } else {
            std::cerr << "Error while disconnecting: " << ec.message() << std::endl;
        }
        connected = false;
    }
}

void Client::send(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex);
    if (connected) {
        boost::asio::write(socket, boost::asio::buffer(message + '\0'));
    } else {
        std::cerr << "Cannot send message, not connected to server" << std::endl;
    }
}

std::string Client::receive() {
    std::lock_guard<std::mutex> lock(mutex);
    if (connected) {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, '\0');
        std::istream is(&buffer);
        std::string message;
        std::getline(is, message, '\0');
        return message;
    }
    return "";
}

void Client::run() {
    std::thread inputThread([this]() {
        while (true) {
            std::string input;
            std::getline(std::cin, input);
            if (input == "exit") {
                disconnect();
                break;
            }
            send(input);
        }
    });

    std::thread receiveThread([this]() {
        while (connected) {
            std::string message = receive();
            if (!message.empty()) {
                std::cout << "Received: " << message << std::endl;
            }
        }
    });

    inputThread.join();
    receiveThread.join();
}
