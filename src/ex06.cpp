// ex06.cpp
// WiFi access point

#include <main.h>

const char *ssid = "ethan esp32";
const char *password = "ethan";
WebServer server(80);

void getIndex() {
  String html = "<html><body><h1>ethan esp32</h1></body></html>";
  server.send(200, "text/html", html);
}

void handle_NotFound(){
  server.send(404, "text/plain", "404");
}


void setup06() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);

  server.on("/", getIndex); 
  server.onNotFound(handle_NotFound);
  server.begin();
}

void loop06() {
  server.handleClient();
}
