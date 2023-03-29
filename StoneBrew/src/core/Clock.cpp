#include <Arduino.h>
#include "Clock.h"

void Clock::begin() {
	rtc.begin();
  lastTick = millis();
  Serial.println("clock started at: " + lastTick);
}

void Clock::tick() {
	lastTick = millis();
}

String Clock::getTime() {
	String format = "+" + String(lastTick) + " ("
	 							+ String(rtc.getHours()) + ":"
						    + String(rtc.getMinutes()) + ":"
						    + String(rtc.getSeconds()) + ")";
	return format;
}

boolean Clock::hasBeen(unsigned int period) {
	return millis() - lastTick >= period;
}
