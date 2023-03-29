
// void route(unsigned long t, String method, String path) {
//   if (method == "GET") {
//     if (path == "/log") {
//       log();
//     } else if (path == "/status") {
//       status(t);
//     } else if (path == "/") {
//       index();
//     } else {
//       not_found();
//     }
//   } else if (method == "POST") {
//     if (path == "/mode") {
//       mode();
//     } else if (path == "/thermostat") {
//       thermostat();
//     }
//   }
// }


void index() {
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

  server.println(httpStatus(PROTOCOL, 200, "OK"));
  server.print("Content-Length: ");
  server.println(content.length() + 2);
  server.println("Content-Type: text/html; charset=utf-8");
  server.println();
  server.println(content);
}
