#include <OneWire.h>
#include <DallasTemperature.h>

#include <ArduinoBLE.h>

#include <WiFiNINA.h>
#include "password.h"  // TODO: Get creds from bluetooth.

String PROTOCOL = "HTTP/1.1";

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Number of temperature sensors on the same bus.
int tBusSize;
// We'll use this variable to store a found device address.
DeviceAddress tempDeviceAddress;

BLEService tempService("180A");
BLEFloatCharacteristic temperature0("433083e7-77f6-434f-b0fd-a365e3c5b8aa", BLERead | BLENotify);
BLEFloatCharacteristic temperature1("f4c587a4-ea04-4ee2-b8ad-766714d993a8", BLERead | BLENotify);
BLEFloatCharacteristic temperature2("e65fe563-82fe-41f6-b2cd-41121830d884", BLERead | BLENotify);
BLEFloatCharacteristic thermostat("67e2e516-ba2a-40dd-8e2a-e273eae78c61", BLERead | BLEWrite);

int wifi_status = WL_IDLE_STATUS;  // the Wi-Fi radio's status

WiFiServer server(80);

unsigned long prevTime;

void setup() {
  Serial.begin(9600);
  setupBluetooth();
  setupWiFi();
  setupServer();
}

void loop() {
  bluetoothLoop();
  wifiLoop();
}


void setupBluetooth() {
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");
    return;
  }

  BLE.setLocalName("Nano 33 IoT");
  BLE.setAdvertisedService(tempService);
  tempService.addCharacteristic(temperature0);
  tempService.addCharacteristic(temperature1);
  tempService.addCharacteristic(temperature2);
  BLE.addService(tempService);

  temperature0.writeValue(12.2);
  temperature1.writeValue(14);
  temperature2.writeValue(137);

  // TODO: Don't always allow connecting.
  BLE.advertise();
  Serial.println("BTLE Advertise");
}

void setupWiFi() {
  while (wifi_status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    wifi_status = WiFi.begin(ssid, password);
    // wait 5 seconds for connection.
    delay(5000);
  }

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("signal strength (RSSI):");
  Serial.println(WiFi.RSSI());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}

void setupServer() {
  server.begin();
  prevTime = millis();

  sensors.begin();
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
      Serial.print(formatAddress(tempDeviceAddress));
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}

String formatStatus(String protocol, unsigned int code, String reason) {
  String status = String("HTTP/1.1") + String(" ") + String(code) + String(" ") + reason;
  return status;
}

String formatAddress(DeviceAddress deviceAddress) {
  String format = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) format += "0";
    format += deviceAddress[i];
  }
  return format;
}

String formatKV(String k, String v) {
  String format = "";
  // format.concat('"');
  format += k;
  // format.concat('"');
  format += ": ";
  format += v;
  return format;
}

void bluetoothLoop() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    if (central.connected()) {
      if (thermostat.written()) {
        float setpoint = thermostat.value();
        Serial.print("Thermostat set to: ");
        Serial.println(setpoint);
      }
    }
  }
}

void wifiLoop() {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      unsigned long currTime = millis();

      Serial.println("Connected to client");

      // Parse HTTP status line.
      String method = client.readStringUntil(' ');
      Serial.print(method);
      String path = client.readStringUntil(' ');
      Serial.print(" ");
      Serial.print(path);
      String protocol = client.readStringUntil('\n');
      Serial.print(" ");
      Serial.println(protocol);
      // TODO: assert protocol == PROTOCOL.

      // Dump the rest which are headers.
      String headers = client.readString();
      Serial.println("-----");
      Serial.println(headers);

      route(currTime, method, path);
      prevTime = currTime;
    }

    client.stop();
  }
}

void route(unsigned long t, String method, String path) {
  if (method == "GET") {
    if (path == "/log") {
      logRoute();
    } else if (path == "/status") {
      statusRoute(t);
    } else if (path == "/") {
      indexRoute();
    } else {
      not_foundRoute();
    }
  } else if (method == "POST") {
    if (path == "/mode") {
      modeRoute();
    } else if (path == "/thermostat") {
      thermostatRoute();
    }
  }
}

void modeRoute() {
  // TODO: Enumerate modes: awake (default), asleep, ...
  Serial.println("Setting Mode: ...");
  server.println(formatStatus(PROTOCOL, 500, "ERR"));
}

void thermostatRoute() {
  // TODO: what variables should we allow to control the device?
  // group head vs boiler (internal / external).
  Serial.println("Setting thermostat...");
  server.println(formatStatus(PROTOCOL, 500, "ERR"));
}

// TODO (history): collect high resolution metered samples, then export them in
// sliding overlappsing windows.
void statusRoute(unsigned long t) {
  server.println(formatStatus(PROTOCOL, 200, "OK"));

  // unsigned long now = WiFi.getTime();  // UNIX time.
  // TODO: Broken multiple threshhold.
  if (t - prevTime >= 1000) {
    prevTime = t;
    sensors.requestTemperatures();
  }

  // String measurement = "{";
  String measurement = "";
  measurement += formatKV("time", String(prevTime));

  for (int i = 0; i < tBusSize; i++) {
    measurement += ", ";

    DeviceAddress addr;
    *addr = sensors.getAddress(tempDeviceAddress, i);
    if (addr) {
    // if (sensors.getAddress(tempDeviceAddress, i)) {
      String key = "";
      // key.concat('#');
      key.concat((char) i + 97);
      // key += " (" + formatAddress(addr) + ")";
      measurement += formatKV(key, String(sensors.getTempC(tempDeviceAddress)));
    }
  }
  // measurement += " }";

  Serial.println("");
  Serial.println(measurement);

  server.println("Content-Type: application/json");
  server.print("Content-Length: ");
  server.println(measurement.length() + 2);
  server.println();
  server.println(measurement);
}

void logRoute() {
  String history = "{},{},{}";
  server.println(formatStatus(PROTOCOL, 200, "OK"));
  server.println("Content-Type: application/json");
  server.print("Content-Length: ");
  server.println(history.length() + 2);
  server.println();
  server.println(history);
}

void indexRoute() {
  String content = " \
    <form action=\"/thermostat\" method=\"post\"> \
      <label for=\"profile\">Choose a thermostat profile:</label> \
      <select name=\"profile\"> \
        <option value=\"boiler\">Boiler</option> \
        <option value=\"group-head\">Group Head</option> \
        <option value=\"Hybrid\">Hybrid</option> \
      </select> \
      <br /> \
      <label for=\"value\">Value:</label> \
      <input type=\"number\" name=\"value\" min=\"0\" max=\"100\" /> \
      <button type=\"submit\">Set</button> \
    </form> \
    \
    <form action=\"/mode\" method=\"post\"> \
      <input type=\"submit\" name=\"mode\" value=\"Sleep\" /> \
      <input type=\"submit\" name=\"mode\" value=\"Wake\" /> \
    </form> \
    ";

  server.println(formatStatus(PROTOCOL, 200, "OK"));
  server.print("Content-Length: ");
  server.println(content.length() + 2);
  server.println("Content-Type: text/html; charset=utf-8");
  server.println();
  server.println(content);
}

void not_foundRoute() {
  server.println(formatStatus(PROTOCOL, 404, "NOT FOUND"));
}
