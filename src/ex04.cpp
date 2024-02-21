// ex04.cpp
// Flash 9 lEDs

#include <main.h>

int LEDs[8] = {5,6,9,10,11,12,13,17};

void setup04() {
  for (int i = 0; i < 8; i++) {
    pinMode(LEDs[i], OUTPUT);
  }
}

void loop04() {
  for (int i = 0; i < 8; i++) {
    Serial.println(LEDs[i]);
    digitalWrite(LEDs[i], HIGH);
    delay(500);
    digitalWrite(LEDs[i], LOW);
  }

  delay(500);

  for (int i = 7; i >= 0; i--) {
    Serial.println(LEDs[i]);
    digitalWrite(LEDs[i], HIGH);
    delay(500);
    digitalWrite(LEDs[i], LOW);
  }

  delay(500);
}
