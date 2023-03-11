#include <WiFiNINA.h>
#include "password.h"
s
String PROTOCOL = "HTTP/1.1";

int wifi_status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  setupWiFi();
  setupServer();
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
}

String httpStatus(String protocol, unsigned int code, String reason) {
  String status = String("HTTP/1.1") + String(" ") + String(code) + String(" ") + reason;
  return status;
}

void loop() {
  WiFiClient client = server.available();
  if (client) {

    if (client.connected()) {
      Serial.println("Connected to client");

      String method = client.readStringUntil(' ');
      Serial.print(method);
      String path = client.readStringUntil(' ');
      Serial.print(" ");
      Serial.print(path);
      String protocol = client.readStringUntil('\n');
      Serial.print(" ");
      Serial.println(protocol);
      // TODO: assert protocol == PROTOCOL.

      String headers = client.readString();
      Serial.println("-----");
      Serial.println(headers);

      route(method, path);
    }

    client.stop();
  }
}

void route(String method, String path) {
  if (method == "GET") {
    if (path == "/" || path == "/status") {
      return status();
    } else if (path == "/log") {
      return log();
    }
  }

  return not_found();
}

void status() {
  server.println(httpStatus(PROTOCOL, 200, "OK"));
  server.println("Content-Type: application/json");
  server.print("Content-Length: ");
  server.println(2 + 2);
  server.println();
  server.println("{}");
}

void log() {
  // TODO
}

void not_found() {
  server.println(httpStatus(PROTOCOL, 404, "NOT FOUND"));
}
