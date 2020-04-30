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

NetworkHandler* server;
LedHandler* ledManager;
Manager* tratPiManager;

int main()
{
	server = new NetworkHandler(5555);
	ledManager = new LedHandler(RED_PIN, GREEN_PIN, BLU_PIN);
	tratPiManager = new Manager(ledManager);

	const string temperaturePath("/sys/bus/w1/devices/28-020d9245dc47/w1_slave");

	ifstream file;
	cerr << "\nTratPi Server (C++ 11)\n";
	server->initializeSocket();

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
		if (server->acceptConnection())
		{
			string command;
			char buffer[64];

			thread temperatureThread(&Manager::temperatureThread, tratPiManager, temperaturePath, server);
			temperatureThread.detach();

			cerr << "Connected to: " << inet_ntoa(server->getClientAddress().sin_addr) << endl;
			do
			{
				int bytesLength = recv(server->getClientSocket(), buffer, sizeof(buffer), 0);
				string message(buffer, bytesLength);
				cerr << "Command: " << message << endl;
				tratPiManager->checkCommandToExecute(command);
			} while (command.compare("disconnecting") != 0);

			server->setConnected(false);

			cerr << "Rack-pc disconnected\n";
		}
	} while (true);
	return 0;
}

void CloseService(int signal)
{
	cerr << "\nExecuting last procedures...\n";
	cerr << "Closing service\n";
	server->writeToClient("disconnecting");
	ledManager->killRainbowThread();
	usleep(500000);
	gpioTerminate();
	server->~NetworkHandler();
	exit(0);
}