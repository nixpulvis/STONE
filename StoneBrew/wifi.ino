#include "password.h"

int wifi_status = WL_IDLE_STATUS;

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

// WiFiClient client = server.available();
// if (client) {
//   if (client.connected()) {
//     Request req = readRequest(client);
//     printRequest(req);
//     route(currTime, req.method, req.path);
//   }
//   client.stop();
// }
