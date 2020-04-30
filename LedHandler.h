#pragma once
#include <mutex>
#include <pigpio.h>
#include <iostream>
#include <math.h>
using namespace std;

class LedHandler
{
private:
	recursive_mutex lock_mutex;
	int redPin, greenPin, bluPin;
	bool stopRainbow;
public:
	void rainbowThread(int);
	int offDelayFunction(int);
	void killRainbowThread();

	LedHandler(int, int, int);
	~LedHandler();
};
