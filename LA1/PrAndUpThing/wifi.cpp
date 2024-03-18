#include <PrAndUpThing.h>

WebServer server(80);

const char *ssid = "ethan tahir wifi";
const char *password = "123456789";


void sendHTML(String bodyText) {
    String html = "<html>"
    "<head>"
    "<title>ethan and tahir</title>"
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

    sendHTML("\nConnecting to " + ssid + "... You can close this page.");
    
    Serial.println("Connecting to WiFi with SSID: " + ssid + "...");
    WiFi.begin(ssid.c_str(), key.c_str());
    // try for 5 seconds
    for (int i = 0; i < 5; i++) {
        delay(1000);
        if (WiFi.status() == WL_CONNECTED) {
            digitalWrite(WIFI_LED, HIGH);
            return;
        }
    }
    Serial.println("Failed to connect to WiFi, it may be out of range or the password is incorrect...");
    WiFi.disconnect();
    
}


void handle_Index() {
    String html = "<h1>List of Nearby WiFi Networks</h1>";
    html += getListNetworks();
    sendHTML(html);
}


void handle_NotFound(){
    server.send(404, "text/plain", "404");
}


void setupWiFi() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid, password);
    WiFi.disconnect();

    server.on("/", handle_Index); 
    server.on("/connect", getConnect);
    server.onNotFound(handle_NotFound);
    server.begin();

    delay(4000);
    Serial.println("Visit 192.168.4.1 to connect to a WiFi network");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Waiting for a WiFi Connection...");
        server.handleClient();

        digitalWrite(NOTIF_LED, HIGH);
        delay(500);
        digitalWrite(NOTIF_LED, LOW);
        delay(500);
    }

    Serial.println("Connected.\n");
    Serial.println();

    digitalWrite(WIFI_LED, HIGH);
}
