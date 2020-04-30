#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(int portNumber)
{
	this->portNumber = portNumber;
}

NetworkHandler::~NetworkHandler()
{
	close(clientSocket);
	close(serverSocket);
}

void NetworkHandler::writeToClient(string message)
{
	if (clientSocket == -1) return;
	message = message + "\n";
	char* wbuff = (char*)message.c_str();
	int wbytes = write(clientSocket, wbuff, strlen(wbuff));
	if (wbytes < 0)
		cerr << "Can't write to socket: " << strerror(errno) << endl;
}

bool NetworkHandler::acceptConnection()
{
	if (!connected)
	{
		clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientAddressLength);
		if (clientSocket == -1)
		{
			cerr << "Connection failed" << endl;
			connected = false;
			return false;
		}
		connected = true;
		return true;
	}
	connected = false;
	return false;
}

void NetworkHandler::initializeSocket()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		cerr << "Failed to create socket\n";
		exit(1);
	}

	// Interface and listening Port
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons((uint16_t)portNumber);
	address.sin_addr.s_addr = inet_addr("0.0.0.0");

	// Asking OS to install the service
	int enable = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		cerr << "Service REUSEADDR failed" << endl;

	if (bind(serverSocket, (sockaddr*)&address, sizeof(address)) == -1)
	{
		cerr << "Service installation failed (bind)\n";
		exit(2);
	}

	// Activating listening on the service created with max 2 pending request
	if (listen(serverSocket, 2) == -1)
	{
		cerr << "Socket listening activation failed (listen)\n";
		exit(3);
	}

	memset(&clientAddress, 0, sizeof(sockaddr_in));
	clientAddressLength = sizeof(sockaddr_in);
}

bool NetworkHandler::isConnected()
{
	lock_guard<recursive_mutex> lock(lock_mutex);
	return connected;
}

sockaddr_in NetworkHandler::getClientAddress()
{
	lock_guard<recursive_mutex> lock(lock_mutex);
	return clientAddress;
}

int NetworkHandler::getClientSocket()
{
	lock_guard<recursive_mutex> lock(lock_mutex);
	return clientSocket;
}

int NetworkHandler::getServerSocket()
{
	lock_guard<recursive_mutex> lock(lock_mutex);
	return serverSocket;
}

void NetworkHandler::setConnected(bool connected)
{
	lock_guard<recursive_mutex> lock(lock_mutex);
	this->connected = connected;
}