#include "ProjectThing.h"

volatile bool timerFired = false;
volatile long startTime = 0;
volatile long endTime = 0;
volatile long distance = 0;

hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer()
{
    timerFired = true;
}

void IRAM_ATTR echoPinISR()
{
    if (digitalRead(echoPin) == HIGH)
    {
        startTime = micros();
    }
    else
    {
        endTime = micros();
        long duration = endTime - startTime;
        distance = duration / 58;
    }
}

void getDistance()
{
    attachInterrupt(digitalPinToInterrupt(echoPin), echoPinISR, CHANGE);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
}

void setupUltrasonicSensor()
{
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 50000, true);
    timerAlarmEnable(timer);
}

void loopUltrasonicSensor()
{
    if (timerFired)
    {
        getDistance();

        if (distance < 20)
        {
            digitalWrite(redLED, HIGH);
            digitalWrite(greenLED, LOW);
        }
        else
        {
            digitalWrite(greenLED, HIGH);
            digitalWrite(redLED, LOW);
        }

        ws.textAll(String(distance));
        timerFired = false;
    }
}
