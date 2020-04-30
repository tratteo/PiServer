#include "LedHandler.h"

LedHandler::LedHandler(int redPin, int greenPin, int bluPin)
{
	this->redPin = redPin;
	this->greenPin = greenPin;
	this->bluPin = bluPin;
	stopRainbow = false;
}

void LedHandler::killRainbowThread()
{
	lock_guard<recursive_mutex> lock(lock_mutex);
	stopRainbow = true;
}

LedHandler::~LedHandler()
{
}

void LedHandler::rainbowThread(int rate)
{
	if (rate == 0)
		rate = 10;

	cout << "Rainbow running with delay: " << rate << "ms" << endl;

	rate = rate * 100;

	int red = 0;
	int green = 0;
	int blu = 0;

	for (; red < 255; red++)
	{
		gpioPWM(redPin, red);
		if (!stopRainbow)
			gpioDelay(rate / 3);
	}
	while (!stopRainbow)
	{
		for (; green < 255; green++)
		{
			gpioPWM(redPin, red);
			gpioPWM(greenPin, green);
			gpioPWM(bluPin, blu);
			if (!stopRainbow)
				gpioDelay(rate);
		}
		for (; red > 0; red--)
		{
			gpioPWM(redPin, red);
			gpioPWM(greenPin, green);
			gpioPWM(bluPin, blu);
			if (!stopRainbow)
				gpioDelay(rate);
		}
		if (!stopRainbow)
			gpioDelay(rate * 10);
		for (; blu < 255; blu++)
		{
			gpioPWM(redPin, red);
			gpioPWM(greenPin, green);
			gpioPWM(bluPin, blu);
			if (!stopRainbow)
				gpioDelay(rate);
		}
		for (; green > 0; green--)
		{
			gpioPWM(redPin, red);
			gpioPWM(greenPin, green);
			gpioPWM(bluPin, blu);
			if (!stopRainbow)
				gpioDelay(rate);
		}
		if (!stopRainbow)
			gpioDelay(rate * 10);
		for (; red < 255; red++)
		{
			gpioPWM(redPin, red);
			gpioPWM(greenPin, green);
			gpioPWM(bluPin, blu);
			if (!stopRainbow)
				gpioDelay(rate);
		}
		for (; blu > 0; blu--)
		{
			gpioPWM(redPin, red);
			gpioPWM(greenPin, green);
			gpioPWM(bluPin, blu);
			if (!stopRainbow)
				gpioDelay(rate);
		}
		if (!stopRainbow)
			gpioDelay(rate * 15);
	}
	//smooth turn off
	for (int max = 255; max > 0; max--)
	{
		if (red >= 0)
		{
			gpioPWM(redPin, red);
			red--;
		}
		if (green >= 0)
		{
			gpioPWM(greenPin, green);
			green--;
		}
		if (blu >= 0)
		{
			gpioPWM(bluPin, blu);
			blu--;
		}
		gpioDelay(offDelayFunction(rate));
		//context->SetRGBValues(0, 0, 0);
	}
}

int LedHandler::offDelayFunction(int rate)
{
	double y = (exp(((rate / 700) * -1) + log(95)) + 5) / 100;
	return (int)(rate * y);
}