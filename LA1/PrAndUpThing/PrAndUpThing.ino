// PrAndUpThing.ino

#include <PrAndUpThing.h>
 

void setup() {
    Serial.begin(115200);

    pinMode(NOTIF_LED, OUTPUT);
    pinMode(WIFI_LED, OUTPUT);
    pinMode(P1_LED, OUTPUT);
    pinMode(P2_LED, OUTPUT);
    pinMode(P3_LED, OUTPUT);
    pinMode(P4_LED, OUTPUT);
    pinMode(P5_LED, OUTPUT);
    
    setupWiFi();
    setupOTA();
}


void loop() {
    if (updateAvailable && touchRead(T14) > 80000) {
        Serial.println("Touch detected, starting update");
        startOTA();
    }
}
