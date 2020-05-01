#pragma once
#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <pigpio.h>
#include <cstring>
using namespace std;
class NetworkHandler
{
private:
	//mutex for thread safety
	recursive_mutex lock_mutex;

	bool connected;
	int portNumber;
	int serverSocket;

	int clientSocket;
	sockaddr_in clientAddress;
	socklen_t clientAddressLength;

public:

	NetworkHandler(int);
	~NetworkHandler();

	void writeToClient(string message);
	bool acceptConnection();
	void initializeSocket();

	//Getters
	bool isConnected();
	sockaddr_in getClientAddress();
	int getClientSocket();
	int getServerSocket();

	//Setters
	void setConnected(bool);
};
