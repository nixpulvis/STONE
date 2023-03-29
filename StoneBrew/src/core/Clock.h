#ifndef CLOCK_h
#define CLOCK_h

#include <Arduino.h>
#include <RTCZero.h>

class Clock {
public:
	unsigned int lastTick;;

	void begin();
	void tick();
	String getTime();
	boolean hasBeen(unsigned int period);
private:
	RTCZero rtc;
};

#endif
