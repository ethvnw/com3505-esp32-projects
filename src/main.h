// main.h
// Exercise declarations and prototypes

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

void setup01(); void loop01();
void setup02(); void loop02();
void setup03(); void loop03();
void setup04(); void loop04();
void setup06(); void loop06();
void setup10(); void loop10();

// Ex01
extern char MAC_ADDRESS[];
void getMAC(char *);

// Ex06
#include <WiFi.h>
#include <WebServer.h>
void getIndex();
void handle_NotFound();


// Ex10
#include <HTTPClient.h>
#include <Update.h>
extern int FIRMWARE_VERSION;

int cloudGet(HTTPClient *, String);
void handleOTAProgress(size_t, size_t);

#endif
