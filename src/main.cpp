#include <Arduino.h>

#define RED 6
#define YELLOW 9
#define GREEN 10
#define SWITCH_PIN 5
char MAC_ADDRESS[13];


void getMAC(char *buf) { // the MAC is 6 bytes, so needs careful conversion...
  uint64_t mac = ESP.getEfuseMac(); // ...to string (high 2, low 4):
  char rev[13];
  sprintf(rev, "%04X%08X", (uint16_t) (mac >> 32), (uint32_t) mac);

  // the byte order in the ESP has to be reversed relative to normal Arduino
  for(int i=0, j=11; i<=10; i+=2, j-=2) {
    buf[i] = rev[j - 1];
    buf[i + 1] = rev[j];
  }
  buf[12] = '\0';
}


void setup() {
  Serial.begin(115200);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  pinMode(SWITCH_PIN, INPUT_PULLUP);

  getMAC(MAC_ADDRESS);
}


void loop() {
  // Serial.printf("MAC Address: %s\n", MAC_ADDRESS);
  // delay(1000);

  if (digitalRead(SWITCH_PIN) == LOW) {
    Serial.println("Switch is pressed");
    digitalWrite(RED, HIGH);
    delay(1000);

    digitalWrite(YELLOW, HIGH);
    delay(1000);

    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, HIGH);
    delay(1000);
    
    digitalWrite(GREEN, LOW);
  } 

  
}
