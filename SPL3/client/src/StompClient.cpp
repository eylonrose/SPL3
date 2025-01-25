#include <iostream>
#include <sstream>
#include "keyboardInput.h"
#include <thread>
#include "StompProtocol.h"
#include "SharedQueue.h"
#include <mutex>
#include <atomic>
#include <fstream>

bool connectionStatus = true;
std::mutex Mutex;
StompProtocol *protocol;
ConnectionHandler* connectionHandler = nullptr;
std::atomic<bool> RunThread(true);


std::vector<std::string> splitMessages(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t pos = 0;
    size_t delimiterPos;

    // Loop to find each occurrence of the delimiter and extract the substring before it
    while ((delimiterPos = str.find(delimiter, pos)) != std::string::npos) {
        result.push_back(str.substr(pos, delimiterPos - pos));
        pos = delimiterPos + delimiter.length();
    }
    
    // Add the last part after the last delimiter (if any)
    result.push_back(str.substr(pos));

    return result;
}

bool sendToServer (std::string toSend){
	if (!connectionHandler->sendFrameAscii(toSend, '\0')) {
		RunThread = false;
		connectionStatus = false;
		return false;
	}
	return true;
}

void handleInputFromUser() {
    while (true) {
        
        std::string userMessage;
        std::getline(std::cin, userMessage);
        if(userMessage.substr(0, 7) == "summary")
        {
            protocol->summary(userMessage);
        }
        else
        {   
		    std::string toSend = protocol->process(userMessage);
		    if (!toSend.empty()) {
			    if (toSend.substr(0, 4) == "SEND") {
                std::vector<std::string> messages = splitMessages(toSend, "next");
                for(std::string message : messages)
                {
                    if(!message.empty())
                    {
                        sendToServer(message);
                    }   
                }
            }
                else{
				    sendToServer(toSend);
			    }
		}
        }
        if(!protocol->getLoggedIn())
        {
            break;
        }
    }
}


void socketThread() {
    while (RunThread) {
        std::string serverResponse;
        if (!connectionHandler->getFrameAscii(serverResponse, '\0')) {
			RunThread = false;
			connectionStatus = false;
			break;
        }		
		protocol->processServer(serverResponse);
        if(!protocol->getLoggedIn())
        {
            RunThread = false;
        }
    }
}


int main(int argc, char *argv[]) {
    /*SharedQueue* sharedQueue = new SharedQueue();
    KeyboardInput* keyboardInput = new KeyboardInput(sharedQueue);
    StompProtocol* stompProtocol = new StompProtocol(sharedQueue);
    std::thread keyboardThread(*keyboardInput);
    std::thread stompThread(*stompProtocol);
    keyboardThread.join();
    stompThread.join();
    return 0;*/

    while(connectionStatus) {
        protocol = new StompProtocol();
		RunThread = true;
		std::string firstWord, secondWord;
		std::string userMessage;
		
		while (firstWord != "login") {
			std::getline(std::cin, userMessage);
			std::istringstream stream(userMessage);
			stream >> firstWord >> secondWord;
		}
		size_t colonPos = secondWord.find(':');
		std::string host = secondWord.substr(0, colonPos);
		std::string portStr = secondWord.substr(colonPos + 1);
		int portInt = std::stoi(portStr);
		short port = static_cast<short>(portInt);
		connectionHandler = new ConnectionHandler(host, port);
		if (!connectionHandler->connect()) {
			std::cerr << "Unable to connect." << std::endl;
			delete connectionHandler;
            delete protocol;
			return 1;
		}

        std::string toSend = protocol->process(userMessage);

        std::thread serverSocketThread(socketThread);
		sendToServer(toSend);	
		handleInputFromUser();

		serverSocketThread.join();

		delete connectionHandler;
        delete protocol;
    }

    std::cout << "Disconnected. Exiting..." << std::endl;



    return 0;
}
