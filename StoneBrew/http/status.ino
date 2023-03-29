
// TODO (history): collect high resolution metered samples, then export them in
// sliding overlappsing windows.
void status(unsigned long t) {
  server.println(formatStatus(PROTOCOL, 200, "OK"));

  // String measurement = getTemps();
  String m = measurements[measurement];

  Serial.println("");
  Serial.println(m);

  server.println("Content-Type: application/json");
  server.print("Content-Length: ");
  server.println(m.length() + 2);
  server.println();
  server.println(m);
}
