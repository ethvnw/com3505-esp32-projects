#include <PrAndUpThing.h>

HTTPClient http;

#define FIRMWARE_IP "192.168.207.2"
#define FIRMWARE_PORT "8000"

bool updateAvailable = false;
int FIRMWARE_VERSION = 1;
int newFirmwareVersion;
int percentLength = 0;


int cloudGet(HTTPClient *http, String file) {
    String url = "http://" + String(FIRMWARE_IP) + ":" + String(FIRMWARE_PORT) + "/" + file;

    http->begin(url);
    http->addHeader("User-Agent", "ESP32");
    return http->GET();
}


void handleOTAProgress(size_t done, size_t total) {
    float progress = (float) done / (float) total;
    String percent = String(progress * 100);
    
    String backspace = "";
    for (int i = 0; i <= percentLength; i++) {
        backspace += "\b";
    }

    if (progress > 0.2)
        digitalWrite(P1_LED, HIGH);
    
    if (progress > 0.4)
        digitalWrite(P2_LED, HIGH);
    
    if (progress > 0.6)
        digitalWrite(P3_LED, HIGH);
    
    if (progress > 0.8)
        digitalWrite(P4_LED, HIGH);
    
    if (progress == 1) {
        digitalWrite(P5_LED, HIGH);

        for (int i = 0; i < 5; i++) {
            delay(500);

            digitalWrite(P1_LED, HIGH);
            digitalWrite(P2_LED, HIGH);
            digitalWrite(P3_LED, HIGH);
            digitalWrite(P4_LED, HIGH);
            digitalWrite(P5_LED, HIGH);
            
            delay(500);
            
            digitalWrite(P1_LED, LOW);
            digitalWrite(P2_LED, LOW);
            digitalWrite(P3_LED, LOW);
            digitalWrite(P4_LED, LOW);
            digitalWrite(P5_LED, LOW);
        }
    }

    percentLength = percent.length();

    Serial.print(backspace);
    Serial.printf("%s%%", percent.c_str());
}


void startOTA() {
    HTTPClient http;
    String firmwareName = String(newFirmwareVersion) + ".bin";
    int firmwareResponseCode = cloudGet(&http, firmwareName);
    int updateLength = http.getSize();

    if (firmwareResponseCode == 200) {
        Serial.printf("Firmware size is %d\n", updateLength);

    } else  {
        Serial.printf("Failed to get firmware (code %d)\n", firmwareResponseCode);
        http.end();
        return;
    }

    WiFiClient stream = http.getStream();
    Update.onProgress(handleOTAProgress);
    if (Update.begin(updateLength)) {
        Serial.println("Starting OTA");
        Update.writeStream(stream);

        if (Update.end()) {
            Serial.println("OTA done, finishing up");
            Serial.flush();

            if (Update.isFinished()) {
                Serial.println("Update finished, restarting");
                ESP.restart();

            } else
                Serial.println("Update failed");

        } else {
            Serial.printf("OTA error %d\n", Update.getError());
            Serial.flush();
        }

    } else {
        Serial.println("OTA failed to begin");
        Serial.flush();
    }

    http.end();
    stream.flush();
}


void setupOTA() {
    Serial.printf("Running firmware on version %d\n", FIRMWARE_VERSION);
    
    int highestVersion;
    int versionResponseCode = cloudGet(&http, "version");
    
    if (versionResponseCode == 200) {
        highestVersion = http.getString().toInt();
        Serial.printf("Cloud version is %d\n", highestVersion);

    } else 
        Serial.printf("Failed to get cloud version (code %d)\n", versionResponseCode);
    http.end();

    if (versionResponseCode < 0)
        return;

    else if (FIRMWARE_VERSION >= highestVersion) {
        Serial.println("Firmware is up to date");
        return;
    }

    updateAvailable = true;
    newFirmwareVersion = highestVersion;

    Serial.printf("Firmware update available (%d -> %d)\n", FIRMWARE_VERSION, highestVersion);
    Serial.println("Touch the sensor to start the update...");
}
