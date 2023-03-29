#ifndef SEGMENTS_h
#define SEGMENTS_h

#include <Arduino.h>
#include <LedControl.h>

class Segments {
  LedControl lc;
public:
  Segments(int a, int b, int c, int d=1): lc(a,b,c,d) {
      Serial.println("HIT");

      lc.shutdown(0,false);
      lc.setIntensity(0,8);
      lc.clearDisplay(0);
  }
  
  void displayF(float temp);
  static int nthdigit(const int number, const int digit);
};

#endif
