// ex01.cpp
// Print the MAC address to serial

#include <main.h>

char MAC_ADDRESS[13];
void getMAC();

void setup01() {
  getMAC(MAC_ADDRESS);
}

void loop01() {
  Serial.printf("MAC Address: %s\n", MAC_ADDRESS);
  delay(1000);
}

void getMAC(char *buf) {
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
