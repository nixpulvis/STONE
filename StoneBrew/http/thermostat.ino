void thermostat() {
  // TODO: what variables should we allow to control the device?
  // group head vs boiler (internal / external).
  Serial.println("Setting thermostat...");
  server.println(httpStatus(PROTOCOL, 500, "ERR"));
}
