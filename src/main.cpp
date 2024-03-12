// main.cpp

#include <Arduino.h>
#include <main.h>

int FIRMWARE_VERSION = 1;

// Controls the exercise to run
int EX = 99;

void setup() {
  Serial.begin(115200);

  switch(EX) {
    case  1: setup01(); break;
    case  2: setup02(); break;
    case  3: setup03(); break;
    case  4: setup04(); break;
    case  6: setup06(); break;
    case 10: setup10(); break;
    case 99: setupWebhook(); break;
  }
}

void loop() {
  switch(EX) {
    case  1: loop01(); break;
    case  2: loop02(); break;
    case  3: loop03(); break;
    case  4: loop04(); break;
    case  6: loop06(); break;
    case 10: loop10(); break;
    case 99: loopWebhook(); break;
  }
}
