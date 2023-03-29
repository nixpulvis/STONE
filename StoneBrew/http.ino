String PROTOCOL = "HTTP/1.1";

void printRequest(Request req) {
  Serial.print(req.method);
  Serial.print(" ");
  Serial.print(req.path);
  Serial.print(" ");
  Serial.println(req.protocol);
  Serial.println("-----");
  Serial.println(req.headers);
}

Request readRequest(WiFiClient client) {
  Serial.println("Connected to client");

  Request req;
  req.method = client.readStringUntil(' ');
  // Serial.print(method);
  req.path = client.readStringUntil(' ');
  // Serial.print(" ");
  // Serial.print(path);
  req.protocol = client.readStringUntil('\n');
  // Serial.print(" ");
  // Serial.println(protocol);
  // TODO: assert protocol == PROTOCOL.

  req.headers = client.readString();
  // Serial.println("-----");
  // Serial.println(headers);

  return req;
}

String httpStatus(String protocol, unsigned int code, String reason) {
  String status = String("HTTP/1.1") + String(" ") + String(code) + String(" ") + reason;
  return status;
}

String formatStatus(String protocol, unsigned int code, String reason) {
  String status = String("HTTP/1.1") + String(" ") + String(code) + String(" ") + reason;
  return status;
}

// void not_found() {
//   server.println(httpStatus(PROTOCOL, 404, "NOT FOUND"));
// }

// #include "http/index.h"
