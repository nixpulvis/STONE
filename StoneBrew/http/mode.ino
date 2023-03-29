void mode() {
  // TODO: Enumerate modes: awake (default), asleep, ...
  Serial.println("Setting Mode: ...");
  server.println(httpStatus(PROTOCOL, 500, "ERR"));
}
