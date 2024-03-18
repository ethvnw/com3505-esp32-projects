// ex02.cpp
// Blink LED and read switch

#include <main.h>

#define SWITCH 5
#define LED 6

void setup02() {
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop02() {
  if (digitalRead(SWITCH) == LOW) {
    Serial.println("Switch is pressed");
    
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
  }
}
