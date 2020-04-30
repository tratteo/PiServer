#include "Manager.h"

Manager::Manager(LedHandler* ledManager)
{
	this->ledManager = ledManager;
}

Manager::~Manager()
{
}

void Manager::checkCommandToExecute(string command)
{
	list<string> commands = splitStringIntoList(command, ',');

	for (string command : commands)
	{
		//TODO implement
	}
}

list<string> Manager::splitStringIntoList(string command, char separator)
{
	int pos;
	string token;
	list<string> commandList;
	while (command.find(separator) != string::npos)
	{
		pos = command.find(separator);
		token = command.substr(0, pos);
		commandList.push_back(token);
		command = command.substr(pos + 1);
	}
	commandList.push_back(command);
	return commandList;
}

void Manager::temperatureThread(string busPath, NetworkHandler* server)
{
	ifstream file;
	file.open(busPath);
	while (!file && server->isConnected())
	{
		cout << "Error opening temperature file" << endl;
		usleep(4000000);
		file.open(busPath);
	}

	cerr << "Starting temperature thread" << endl;
	while (server->isConnected())
	{
		string tmp, line;
		getline(file, line);
		getline(file, line);
		int equalIndex = line.find("=");
		int intTmp = stoi(line.substr(equalIndex + 1));
		float floatTmp = (float)intTmp / 1000;
		tmp = to_string(floatTmp);
		tmp = tmp.substr(0, tmp.find(".") + 2);
		file.seekg(0, ios::beg);
		usleep(1000000);
		server->writeToClient(tmp);
	}
	file.close();
	cerr << "Temperature thread terminated" << endl;
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