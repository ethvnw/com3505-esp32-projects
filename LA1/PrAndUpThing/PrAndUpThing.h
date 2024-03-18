#ifndef PrAndUpThing_h
#define PrAndUpThing_h

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <Update.h>

#define NOTIF_LED 10
#define WIFI_LED 11

#define P1_LED 3
#define P2_LED 4
#define P3_LED 5
#define P4_LED 6
#define P5_LED 9


// wifi.cpp

void setupWiFi();


// ota.cpp
void setupOTA();

extern bool updateAvailable;
void startOTA();

#endif