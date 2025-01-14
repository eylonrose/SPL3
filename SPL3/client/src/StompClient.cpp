#include "../include/Client.h"
#include <iostream>
#include <sstream> 


int main(int argc, char *argv[]) {
	

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
        return 1;
    }

    std::string host = argv[1];
    int port = std::stoi(argv[2]);

    Client client(host, port);
    if (client.connect()) {
        client.run();
    } else {
        std::cerr << "Failed to start client" << std::endl;
    }

    return 0;
}
