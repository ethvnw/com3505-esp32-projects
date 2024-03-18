// ex03.cpp
// Traffic lights

#include <main.h>

#define RED 6
#define YELLOW 9
#define GREEN 10
#define SWITCH 5

void setup03() {
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);
}

void loop03() {
  if (digitalRead(SWITCH) == LOW) {
    Serial.println("Switch is pressed");
    digitalWrite(RED, HIGH);
    delay(1000);

    digitalWrite(YELLOW, HIGH);
    delay(1000);

    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, HIGH);
    delay(2000);
    
    digitalWrite(GREEN, LOW);
  } 
}
