#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>
#include <MadgwickAHRS.h>

int wifi_status = WL_IDLE_STATUS;

WiFiServer server(80);

Madgwick filter;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  filter.begin(10);

  // while (wifi_status != WL_CONNECTED) {
  //   Serial.print("Attempting to connect to network: ");
  //   Serial.println(ssid);
  //   wifi_status = WiFi.begin(ssid, password);

  //   // wait 5 seconds for connection.
  //   delay(5000);
  // }

  // Serial.print("SSID: ");
  // Serial.println(WiFi.SSID());
  // Serial.print("signal strength (RSSI):");
  // Serial.println(WiFi.RSSI());
  // Serial.print("IP Address: ");
  // Serial.println(WiFi.localIP());

  // server.begin();
}

void loop() {
  float ax, ay, az, gx, gy, gz;
  String measure;

  if (
    IMU.accelerationAvailable() &&
    IMU.readAcceleration(ax, ay, az) &&
    IMU.gyroscopeAvailable() && 
    IMU.readGyroscope(gx, gy, gz))
  {
    filter.updateIMU(gx, gy, gz, ax, ay, az);  // FIXME: a 6 argument updateIMU function should not drift.

    measure = "(";
    measure += String(filter.getRoll());
    measure += ", ";
    measure += String(filter.getPitch());
    measure += ", ";
    measure += String(filter.getYaw());  // drifty boi.
    measure += ")";
    Serial.println(measure);
  }

  // WiFiClient client = server.available();
  // if (client) {
  //   if (client.connected()) {
  //     Serial.println("connected");
  //     // Serial.println(client.readString());
  //     server.println(measure);
  //   }
  //   client.stop();
  // }
}
