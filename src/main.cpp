// main.cpp

#include <Arduino.h>
#include <main.h>

// Controls the exercise to run
int EX = 3;

void setup() {
  Serial.begin(115200);

  switch(EX) {
    case  1: setup01(); break;
    case  2: setup02(); break;
    case  3: setup03(); break;
  }
}

void loop() {
  switch(EX) {
    case  1: loop01(); break;
    case  2: loop02(); break;
    case  3: loop03(); break;
  }
}
