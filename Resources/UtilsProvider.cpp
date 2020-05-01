#include "UtilsProvider.h"

list<string> UtilsProvider::splitStringIntoList(string input, char separator)
{
	int pos;
	string token;
	list<string> commandList;
	while (input.find(separator) != string::npos)
	{
		pos = input.find(separator);
		token = input.substr(0, pos);
		commandList.push_back(token);
		input = input.substr(pos + 1);
	}
	commandList.push_back(input);
	return commandList;
}