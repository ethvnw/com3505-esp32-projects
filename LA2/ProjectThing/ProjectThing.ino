#include "ProjectThing.h"

void setup()
{
  Serial.begin(115200);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  setupMotorServer();
  setupUltrasonicSensor();
}

void loop()
{
  loopUltrasonicSensor();
}
