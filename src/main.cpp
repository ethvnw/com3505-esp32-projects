#include <Arduino.h>

// put function declarations here:
#define LED_PIN 6
#define SWITCH_PIN 5

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  Serial.println("do i work");

}

void loop() {
  Serial.println("Testing exercise 2");

  if (digitalRead(SWITCH_PIN) == LOW) {
    Serial.println("Button pressed!");
  }

  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
