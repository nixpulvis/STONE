void log() {
  server.println(formatStatus(PROTOCOL, 200, "OK"));

  String result = "";

  for (String measurement : measurements) {
    Serial.println("");
    Serial.println(measurement);
    result += measurement + "\n";
  }

  server.println("Content-Type: application/json");
  server.print("Content-Length: ");
  server.println(result.length() + 2);
  server.println();
  server.println(result);
}
