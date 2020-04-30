#pragma once
#include <string>
#include <fstream>
#include <list>
#include <iostream>
#include <regex>
#include "LedHandler.h"
#include "NetworkHandler.h"
using namespace std;

class Manager
{
private:
	LedHandler* ledManager;
public:
	Manager(LedHandler*);
	~Manager();

	void checkCommandToExecute(string);
	list<string> splitStringIntoList(string, char);

	bool rainbowstart_Match(string);
	void temperatureThread(string, NetworkHandler*);
};
