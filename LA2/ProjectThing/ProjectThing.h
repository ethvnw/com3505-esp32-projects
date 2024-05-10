#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>

const int echoPin = 5;
const int redLED = A3;
const int greenLED = A4;
const int trigPin = A5;

// MotorServer.cpp
extern AsyncWebSocket ws;
void setupMotorServer();

// UltrasonicSensor.cpp
void setupUltrasonicSensor();
void loopUltrasonicSensor();

// ServerFiles.h
extern const char HTML_INDEX[];
extern const char CSS_STYLE[];
extern const char JS_CONTROL[];