#include <cstring>
#include <iostream>
#include <signal.h>
#include <fstream>
#include "NetworkHandler.h"
#include "LedHandler.h"
#include "Manager.h"

#define RED_PIN 22
#define GREEN_PIN 24
#define BLU_PIN 26

using namespace std;

void CloseService(int);

NetworkHandler* networkHandler;
LedHandler* ledManager;
Manager* manager;

int main()
{
	networkHandler = new NetworkHandler(5555);
	ledManager = new LedHandler(RED_PIN, GREEN_PIN, BLU_PIN);
	manager = new Manager(ledManager, networkHandler);

	const string temperaturePath("/sys/bus/w1/devices/28-020d9245dc47/w1_slave");

	ifstream file;
	cerr << "\nTratPi Server (C++ 11)\n";
	networkHandler->initializeSocket();

	//initializing PIGPIO

	if (gpioInitialise() < 0)
	{
		cerr << "Error initialising GPIO PigPio" << endl;
		exit(0);
	}

	signal(SIGINT, CloseService);
	signal(SIGKILL, CloseService);
	signal(SIGTERM, CloseService);

	do
	{
		cerr << "Waiting for rack-pc connection...\n";
		if (networkHandler->acceptConnection())
		{
			string command;
			char buffer[64];

			thread temperatureThread(&Manager::temperatureThread, manager, temperaturePath, 4000000, 1000000);
			temperatureThread.detach();

			cerr << "Connected to: " << inet_ntoa(networkHandler->getClientAddress().sin_addr) << endl;
			do
			{
				int bytesLength = recv(networkHandler->getClientSocket(), buffer, sizeof(buffer), 0);
				string message(buffer, bytesLength);
				cerr << "Received: " << message << endl;
				manager->checkCommandToExecute(command);
			} while (command.compare("disconnecting") != 0);

			networkHandler->setConnected(false);

			cerr << "Rack-pc disconnected\n";
		}
	} while (true);
	return 0;
}

void CloseService(int signal)
{
	manager->closeService(signal);
	exit(0);
}