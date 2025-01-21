all: link

link: compile
	g++ -o bin/StompEMIClient bin/ConnectionHandler.o bin/event.o bin/keyboardInput.o bin/Message.o bin/SharedQueue.o bin/StompClient.o bin/StompProtocol.o -lboost_system -lboost_thread -lboost_chrono -lboost_locale -lboost_iostreams -lboost_date_time -lboost_regex -pthread

compile: src/ConnectionHandler.cpp src/event.cpp src/keyboardInput.cpp src/Message.cpp src/SharedQueue.cpp src/StompClient.cpp src/StompProtocol.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -o bin/event.o -c src/event.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/ConnectionHandler.o src/ConnectionHandler.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/keyboardInput.o src/keyboardInput.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Message.o src/Message.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SharedQueue.o src/SharedQueue.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/StompClient.o src/StompClient.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/StompProtocol.o src/StompProtocol.cpp

run: 
	./bin/StompEMIClient
clean:
	rm -f bin/*
	