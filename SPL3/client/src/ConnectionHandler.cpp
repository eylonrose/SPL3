#include "../include/ConnectionHandler.h"
#include <boost/asio/deadline_timer.hpp>

using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

ConnectionHandler::ConnectionHandler(string host, short port) : host_(host), port_(port), io_service_(),
                                                                socket_(io_service_) {}

ConnectionHandler::~ConnectionHandler() {
	close();
}

bool ConnectionHandler::connect() {
	std::cout << "Starting connect to "
	          << host_ << ":" << port_ << std::endl;
	try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
	}
	catch (std::exception &e) {
		std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
	size_t tmp = 0;
	boost::system::error_code error;
	try {
		while (!error && bytesToRead > tmp) {
			tmp += socket_.read_some(boost::asio::buffer(bytes + tmp, bytesToRead - tmp), error);
		}
		if (error)
			throw boost::system::system_error(error);
	} catch (std::exception &e) {
		std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

/*bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
    boost::system::error_code error;
    boost::asio::deadline_timer timer(io_service_);
    bool timeout = false;

    // Set a deadline for the operation (10 seconds)
    timer.expires_from_now(boost::posix_time::seconds(1));

    // Start the timeout handler (timer callback)
    timer.async_wait([&](const boost::system::error_code &ec) {
        if (!ec) {
            timeout = true;
            socket_.cancel(); // Cancel the socket operation if timeout occurs
        }
    });

    try {
        // Create a buffer and set the callback for the async_read_some operation
        boost::asio::async_read(socket_, boost::asio::buffer(bytes, bytesToRead),
            [&](const boost::system::error_code& ec, std::size_t bytes_transferred) {
                if (ec) {
                    error = ec;
                    return;
                }
                tmp += bytes_transferred;
            });

        // Run io_service to process the asynchronous operation
        io_service_.run();

        // Handle the timeout condition
        if (timeout) {
            //std::cerr << "Socket read timed out" << std::endl;
            return false; // Exit if timeout occurred
        }

        if (error) {
            std::cerr << "recv failed (Error: " << error.message() << ")" << std::endl;
            return false;
        }

    } catch (std::exception &e) {
        std::cerr << "recv failed (Error: " << e.what() << ")" << std::endl;
        return false;
    }

    return true;
}*/


bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
	int tmp = 0;
	boost::system::error_code error;
	try {
		while (!error && bytesToWrite > tmp) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
		}
		if (error)
			throw boost::system::system_error(error);
	} catch (std::exception &e) {
		std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

bool ConnectionHandler::getLine(std::string &line) {
	return getFrameAscii(line, '\n');
}

bool ConnectionHandler::sendLine(std::string &line) {
	return sendFrameAscii(line, '\n');
}


bool ConnectionHandler::getFrameAscii(std::string &frame, char delimiter) {
	char ch;
	// Stop when we encounter the null character.
	// Notice that the null character is not appended to the frame string.
	try {
		do {
			if (!getBytes(&ch, 1)) {
				//std::cout << "getBytes failed" << std::endl;
				return false;
			}
			if (ch != '\0')
				frame.append(1, ch);
		} while (delimiter != ch);
	} catch (std::exception &e) {
		std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

bool ConnectionHandler::sendFrameAscii(const std::string &frame, char delimiter) {
	bool result = sendBytes(frame.c_str(), frame.length());
	if (!result) return false;
	return sendBytes(&delimiter, 1);
}

// Close down the connection properly.
void ConnectionHandler::close() {
	try {
		socket_.close();
	} catch (...) {
		std::cout << "closing failed: connection already closed" << std::endl;
	}
}
