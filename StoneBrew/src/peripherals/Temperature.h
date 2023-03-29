#ifndef TEMPERATURE_h
#define TEMPERATURE_h

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>



class Temperature {
  static const unsigned int MEASUREMENT_WINDOW = 60 * 10;
  // TODO string should be (int, int, ...) and store more
  String measurements[MEASUREMENT_WINDOW];
  int measurement = 0;

public:
  // TODO: Refactor.
  Temperature begin();
  String format(String t);
  float sample(String label);
  void updateMeasurements(String label);
  static String formatAddress(DeviceAddress deviceAddress);
  static String formatKV(String k, String v);
};

#endif
