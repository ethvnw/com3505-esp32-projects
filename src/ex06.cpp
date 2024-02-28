// ex06.cpp
// WiFi access point

#include <main.h>

const char *ssid = "ethn wifi";
const char *password = "123456789";
WebServer server(80);

void sendHTML(String bodyText) {
  String html = "<html>"
  "<head>"
  "<title>ethan esp32</title>"
  "<style>"
  "body { background-color: rgb(20,20,20); font-family: Arial, Helvetica, Sans-Serif; color: rgb(220,220,220); }"
  "</style>"
  "</head>"
  "<body>"
  + bodyText +
  "</body>"
  "</html>";
  server.send(200, "text/html", html);
}

String getListNetworks() {
  int numNetworks = WiFi.scanNetworks();
  String html = "<h2>Networks</h2>";
  for (int i = 0; i < numNetworks; i++) {
    html += "<form action=\"/connect\" method=\"post\">";
    html += "<p>Name:" + WiFi.SSID(i) + "</p>";
    html += "<p>Signal:" + String(WiFi.RSSI(i)) + "</p>";
    html += "<p>Encryption: " + String(WiFi.encryptionType(i)) + "</p>";

    html += "<input type=\"hidden\" name=\"ssid\" value=\"" + WiFi.SSID(i) + "\">";
    html += "<p><input placeholder=\"enter wifi password\" type=\"password\" name=\"password\"></p>";
    html += "<p><input type=\"submit\" value=\"Connect\"></p>";

    html += "</form>";
    html += "<br>";

  }

  return html;
}

void getConnect() {
  String ssid = "";
  String key = "";

  for(uint8_t i = 0; i < server.args(); i++ ) {
    if(server.argName(i) == "ssid")
      ssid = server.arg(i);
    else if(server.argName(i) == "password")
      key = server.arg(i);
  }

  sendHTML("Connecting to " + ssid + "...");
  WiFi.begin(ssid, key);
  delay(1500);
  sendHTML("Connected to " + ssid + "...");
}

void getIndex() {
  String html = "<marquee><h1>index page!!</h1></marquee>";
  html += getListNetworks();
  sendHTML(html);
}

void handle_NotFound(){
  server.send(404, "text/plain", "404");
}


void setup06() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);

  server.on("/", getIndex); 
  server.on("/connect", getConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
}

void loop06() {
  server.handleClient();
  WiFi.printDiag(Serial);
}
