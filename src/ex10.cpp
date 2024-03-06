// ex10.cpp
// OTA updates

#include <main.h>

#define FIRMWARE_IP "192.168.207.2"
#define FIRMWARE_PORT "8000"

int percentLength = 0;

int cloudGet(HTTPClient *http, String file) {
    String url = "http://" + String(FIRMWARE_IP) + ":" + String(FIRMWARE_PORT) + "/" + file;

    http->begin(url);
    http->addHeader("User-Agent", "ESP32");
    return http->GET();
}

void handleOTAProgress(size_t done, size_t total) {
    float progress = (float) done / (float) total;
    
    String backspace = "";
    for (int i = 0; i <= percentLength; i++) {
        backspace += "\b";
    }

    String percent = String(progress * 100);
    percentLength = percent.length();

    Serial.print(backspace);
    Serial.printf("%s%%", percent.c_str());

    if (progress == 1)
        Serial.println("");
}

void setup10() {
    delay(3000);
    Serial.printf("Running firmware on version %d\n", FIRMWARE_VERSION);

    WiFi.begin("MAFair", "1357913579");
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnected to WiFi");

    HTTPClient http;
    int highestVersion;;
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

    Serial.printf("Firmware update available (%d -> %d)\n", FIRMWARE_VERSION, highestVersion);
    String firmwareName = String(highestVersion) + ".bin";
    int firmwareResponseCode = cloudGet(&http, firmwareName);
    int updateLength = http.getSize();

    if (firmwareResponseCode == 200) {
        Serial.printf("Firmware size is %d\n", updateLength);

    } else  {
        Serial.printf("Failed to get firmware (code %d)\n", versionResponseCode);
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

    stream.flush();
}

void loop10() {
    delay(1000);
    Serial.println("Looping");
}
