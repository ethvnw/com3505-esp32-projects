// webhook.cpp
// post to discord

#include <main.h>


// function to send json to discord webhook url
void sendToDiscord(String message) {
    HTTPClient http;
    http.begin("https://discord.com/api/webhooks/1216738036088311829/arGnHDmruFCcFNajp6mHm0LK7HA-dAydQU3fO8rnsmNcpJTOKrgfRisbb-stS-t8fXmV");
    http.addHeader("Content-Type", "application/json");
    http.POST(message);
    http.end();

    Serial.println("Message sent to Discord");
}


void setupWebhook() {
    delay(3000);

    Serial.printf("Running firmware on version %d\n", FIRMWARE_VERSION);

    WiFi.begin("MAFair", "1357913579");
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnected to WiFi");

}


void loopWebhook() {
    if (touchRead(T6) > 100000) {
        Serial.println("Touched");
        sendToDiscord("{\"content\": \"ethn so cool\"}");
        delay(1000);
    }
}
