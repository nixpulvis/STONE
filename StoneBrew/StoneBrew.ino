#include "src/core/Clock.h"
Clock clock;

// #include "src/peripherals/Segments.h"
// Segments segments(5,3,4,1);

#include "src/peripherals/Temperature.h"
Temperature temperature;

#include <WiFiNINA.h>
WiFiServer server(80);

typedef struct Request {
  String method;
  String path;
  String protocol;
  String headers;
} Request;

void server_loop(WiFiServer server, Temperature temperature) {
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

      // log();
    }

    client.stop();
  }
}

// void log() {
//   server.println(formatStatus(PROTOCOL, 200, "OK"));
//
//   String result = "";
//
//   for (String measurement : measurements) {
//     Serial.println("");
//     Serial.println(measurement);
//     result += measurement + "\n";
//   }
//
//   server.println("Content-Type: application/json");
//   server.print("Content-Length: ");
//   server.println(result.length() + 2);
//   server.println();
//   server.println(result);
// }

void setup() {
  Serial.begin(9600);
  Serial.println("SETUP");

  // TODO: Setup and read RTC time then use that to base the millis count.
  clock.begin();
  
  temperature.begin();
  
  server.begin();
  setupWiFi();
}


void loop() {
  server_loop(server, temperature);
  Serial.println("HIT");

  if (clock.hasBeen(500)) {
    // segments.displayF(temperature.sample(clock.getTime()));
    clock.tick();
  }
}
