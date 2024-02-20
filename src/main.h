// main.h
// Exercise declarations and prototypes

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

void setup01(); void loop01();
void setup02(); void loop02();
void setup03(); void loop03();

// Ex01
extern char MAC_ADDRESS[];
void getMAC(char *);

#endif
