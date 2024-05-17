/**
 * Controls the ultrasonic sensor
 */

#include "ProjectThing.h"

volatile bool timerFired = false;
volatile long startTime = 0;
volatile long endTime = 0;
volatile long distance = 0;

hw_timer_t *timer = NULL;

/**
 * Called when the timer fires.
 */
void IRAM_ATTR onTimer()
{
    timerFired = true;
}

/**
 * Called when the echo pin changes state.
 * If the echo pin is high, the start time is recorded.
 * If the echo pin is low, the end time is recorded and the distance is calculated.
 */
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

/**
 * Sends a trigger signal to the ultrasonic sensor.
 */
void getDistance()
{
    attachInterrupt(digitalPinToInterrupt(echoPin), echoPinISR, CHANGE);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
}

/**
 * Sets up the ultrasonic sensor.
 */
void setupUltrasonicSensor()
{
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 50000, true);
    timerAlarmEnable(timer);
}

/**
 * Loops the ultrasonic sensor.
 */
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

        // Sending the distance to the client
        ws.textAll(String(distance));
        timerFired = false;
    }
}
