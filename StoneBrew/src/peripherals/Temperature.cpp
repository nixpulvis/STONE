#include "Temperature.h"

// Number of temperature sensors on the same bus.
int tBusSize;
// We'll use this variable to store a found device address.
DeviceAddress tempDeviceAddress;

OneWire bus(2);
DallasTemperature sensors(&bus);

Temperature Temperature::begin() {
  sensors.begin();
  sensors.setResolution(11);
  tBusSize = sensors.getDeviceCount();

  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(tBusSize, DEC);
  Serial.println(" devices.");
  // Loop through each device, print out address
  for (int i = 0; i < tBusSize; i++) {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      Serial.print(Temperature::formatAddress(tempDeviceAddress));
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }

  return Temperature {};
}

// TODO: Return array of C values.
String Temperature::format(String t) {
  String measurement = "{";
  measurement += Temperature::formatKV("time", String(t));

  for (int i = 0; i < tBusSize; i++) {
    measurement += ", ";
    DeviceAddress addr;
    *addr = sensors.getAddress(tempDeviceAddress, i);
    if (addr) {
      measurement += Temperature::formatKV(String(i), String(sensors.getTempF(tempDeviceAddress)));
      measurement += "F";
    }
  }
  measurement += " }";

  return measurement;
}

float Temperature::sample(String label) {
  // TODO: Don't rerequest unless it's been long enough given the current
  // resolution.
  sensors.requestTemperatures();

  updateMeasurements(label);

  return sensors.getTempF(tempDeviceAddress);
}

void Temperature::updateMeasurements(String label) {
  measurement = (measurement + 1) % MEASUREMENT_WINDOW; // leaves 0th free the first time.
  measurements[measurement] = format(label);
}

String Temperature::formatAddress(DeviceAddress deviceAddress) {
  String format = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) format += "0";
    format += deviceAddress[i];
  }
  return format;
}

String Temperature::formatKV(String k, String v) {
  String format = "";
  // format.concat('"');
  format += k;
  // format.concat('"');
  format += ": ";
  format += v;
  return format;
}
