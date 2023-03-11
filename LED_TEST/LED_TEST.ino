#include <LedControl.h>

LedControl lc=LedControl(2,3,4,1);

void displayTemperature(float temp) {
  lc.setDigit(0, 0, temp/10, false);
  lc.setDigit(0, 1, temp, true);
  lc.setDigit(0, 2, temp*10, false);
  lc.setDigit(0, 3, temp*100, false);
}

void setup() {
  Serial.begin(9600);

  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
}


void loop() {
  lc.clearDisplay(0);
  int t = millis();
  lc.setDigit(0, 0, t >> 0, false);
  lc.setDigit(0, 1, t >> 1, false);
  lc.setDigit(0, 2, t >> 2, true);
  lc.setDigit(0, 3, t >> 3, false);
  delay(500);
}
