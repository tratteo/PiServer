#pragma once
#include <mutex>
#include <pigpio.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
using namespace std;

class LedHandler
{
private:
	recursive_mutex lock_mutex;
	int redPin, greenPin, bluPin;
	bool stopRainbow;
	int currentRate;
public:
	void startRainbow(int);
	int offDelayFunction(int);
	void killRainbow();
	void waitForRainbowKill();

	LedHandler(int, int, int);
	~LedHandler();
};
