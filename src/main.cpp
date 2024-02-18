#include <Arduino.h>

#define LED_PIN 6
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
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  getMAC(MAC_ADDRESS);
}


void loop() {
  Serial.printf("MAC Address: %s\n", MAC_ADDRESS);

  if (digitalRead(SWITCH_PIN) == LOW) {
    Serial.println("Button pressed!");
  }

  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
