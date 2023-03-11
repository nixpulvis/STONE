#include <WiFiNINA.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "password.h"

// Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int numberOfDevices;  // Number of temperature devices found

DeviceAddress tempDeviceAddress;  // We'll use this variable to store a found device address

int status = WL_IDLE_STATUS;  // the Wi-Fi radio's status

WiFiServer server(80);

unsigned long prevTime;

void setup(void) {
  prevTime = millis();

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  // // attempt to connect to Wi-Fi network:
  // while (status != WL_CONNECTED) {
  //   Serial.print("Attempting to connect to network: ");
  //   Serial.println(_SSID);
  //   status = WiFi.begin(_SSID, _PASSWORD);
  //   // wait 5 seconds for connection.
  //   delay(5000);
  //   printWiFi();
  // }
  // server.begin();

  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  // Loop through each device, print out address
  for (int i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}

void loop(void) {
  unsigned long currTime = millis();
  // if (currTime - prevTime >= 17) {
  //   sensors.requestTemperatures();
  // }
  sensors.requestTemperatures();

  if (currTime - prevTime >= 1000) {
    prevTime = currTime;

    for (int i = 0; i < numberOfDevices; i++) {
      if (sensors.getAddress(tempDeviceAddress, i)) {
        Serial.print("#");
        Serial.print(i, DEC);
        Serial.print(":");
        Serial.print(sensors.getTempF(tempDeviceAddress));
        Serial.print(",");
      }
    }
    Serial.println();
  }
  // if (currTime - prevTime >= 5000) {
  //   printWiFi();
  // }

  // WiFiClient client = server.available();
  // if (client) {
  //   if (client.connected()) {
  //     Serial.println("Connected to client");

  //     for (int i = 0; i < numberOfDevices; i++) {
  //       if (sensors.getAddress(tempDeviceAddress, i)) {
  //         server.print(sensors.getTempF(tempDeviceAddress));
  //         server.println(" degrees F");
  //       }
  //     }
  //   }
  //   client.stop();
  // }
}

void printWiFi() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("signal strength (RSSI):");
  Serial.println(WiFi.RSSI());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
