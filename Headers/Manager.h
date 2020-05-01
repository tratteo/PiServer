#pragma once
#include <string>
#include <fstream>
#include <list>
#include <iostream>
#include <regex>
#include "LedHandler.h"
#include "NetworkHandler.h"
#include "UtilsProvider.h"

using namespace std;

class Manager
{
private:
	LedHandler* ledManager;
	NetworkHandler* networkHandler;

	void readTemperatureFile(ifstream*, string*);
public:
	Manager(LedHandler*, NetworkHandler*);
	~Manager();

	void checkCommandToExecute(string);
	void closeService(int);
	bool rainbowstart_Match(string);
	void temperatureThread(string, int, int);
};
