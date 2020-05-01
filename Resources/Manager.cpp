#include "Manager.h"

Manager::Manager(LedHandler* ledManager, NetworkHandler* networkHandler)
{
	this->ledManager = ledManager;
	this->networkHandler = networkHandler;
}

Manager::~Manager()
{
	free(ledManager);
	free(networkHandler);
}

void Manager::checkCommandToExecute(string command)
{
	list<string> commands = UtilsProvider::splitStringIntoList(command, ',');

	for (string command : commands)
	{
		//TODO implement
	}
}

void Manager::closeService(int signal)
{
	cerr << "\nExecuting last procedures...\n";
	cerr << "Closing service\n";
	networkHandler->writeToClient("disconnecting");
	ledManager->waitForRainbowKill();
	gpioTerminate();
	this->~Manager();
}

void Manager::temperatureThread(string busPath, int attemptDelay, int readDelay)
{
	ifstream file;
	string temperature;
	file.open(busPath);
	while (!file && !networkHandler->isConnected())
	{
		cerr << "Error opening temperature file" << endl;
		cerr << "Retrying..." << endl;
		usleep(attemptDelay);
		file.open(busPath);
	}
	cerr << "File correctly opened on bus" << endl << " Starting temperature thread" << endl;

	while (networkHandler->isConnected())
	{
		readTemperatureFile(&file, &temperature);
		networkHandler->writeToClient(temperature);
		usleep(readDelay);
	}
	file.close();
	cerr << "Temperature thread terminated" << endl;
}

void Manager::readTemperatureFile(ifstream* file, string* out)
{
	string line;
	getline(*file, line);
	getline(*file, line);
	int equalIndex = line.find("=");
	int intTmp = stoi(line.substr(equalIndex + 1));
	float floatTmp = (float)intTmp / 1000;
	*out = to_string(floatTmp);
	*out = (*out).substr(0, (*out).find(".") + 2);
	(*file).seekg(0, ios::beg);
}

//matches for commands
bool Manager::rainbowstart_Match(string command)
{
	smatch match;
	regex expr("rainbowstart([1-9]{1}[0-9]*)?");
	if (regex_match(command, match, expr))
		return true;
	return false;
}