/**
 * Handles the motor control server for the RoboCar project.
 */

#include "ProjectThing.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *L_MOTOR = AFMS.getMotor(3);
Adafruit_DCMotor *R_MOTOR = AFMS.getMotor(4);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char *ssid = "RoboCar (visit 192.168.4.1)";
int numClients = 0;
int motorSpeed = 128;

/**
 * Moves the car forward.
 * @param command The command received from the client.
 */
int forward(String command)
{
  R_MOTOR->setSpeed(motorSpeed);
  R_MOTOR->run(FORWARD);
  L_MOTOR->setSpeed(motorSpeed);
  L_MOTOR->run(FORWARD);

  return 1;
}

/**
 * Moves the car backward.
 * @param command The command received from the client.
 */
int backward(String command)
{
  R_MOTOR->setSpeed(motorSpeed);
  R_MOTOR->run(BACKWARD);
  L_MOTOR->setSpeed(motorSpeed);
  L_MOTOR->run(BACKWARD);

  return 1;
}

/**
 * Turns the car left.
 * @param command The command received from the client.
 */
int left(String command)
{
  R_MOTOR->setSpeed(motorSpeed);
  R_MOTOR->run(FORWARD);
  L_MOTOR->setSpeed(motorSpeed);
  L_MOTOR->run(BACKWARD);

  return 1;
}

/**
 * Turns the car right.
 * @param command The command received from the client.
 */
int right(String command)
{
  R_MOTOR->setSpeed(motorSpeed);
  R_MOTOR->run(BACKWARD);
  L_MOTOR->setSpeed(motorSpeed);
  L_MOTOR->run(FORWARD);

  return 1;
}

/**
 * Stops the car moving.
 * @param command The command received from the client.
 */
int stop(String command)
{
  R_MOTOR->setSpeed(0);
  R_MOTOR->run(RELEASE);
  L_MOTOR->setSpeed(0);
  L_MOTOR->run(RELEASE);

  return 1;
}

/**
 * Handles websocket events for motor control.
 *
 * @param server The WebSocket server.
 * @param client The WebSocket client that triggered the event.
 * @param type The type of the event.
 * @param arg Additional argument provided with the event.
 * @param data The data received with the event.
 * @param len The length of the data received.
 *
 */
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  // Client has connected
  case WS_EVT_CONNECT:
    // Prevents more than one client from connecting
    if (numClients == 0)
    {
      Serial.printf("[%u] Connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      numClients++;
    }
    else
    {
      client->close(4000, "A controller is already connected.");
    }

    break;

  // Client has disconnected
  case WS_EVT_DISCONNECT:
    Serial.printf("[%u] Disconnected!\n", client->id());
    if (numClients > 0)
    {
      numClients--;
    }

    break;

  // Commands sent from the client
  case WS_EVT_DATA:
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len)
    {
      // Make sure the entire message is captured and null-terminated
      char *msg = new char[len + 1];
      memcpy(msg, data, len);
      msg[len] = '\0';
      String command = msg;
      delete[] msg;

      if (command == "forward")
      {
        forward(command);
      }
      else if (command == "backward")
      {
        backward(command);
      }
      else if (command == "left")
      {
        left(command);
      }
      else if (command == "right")
      {
        right(command);
      }
      else if (command == "stop")
      {
        stop(command);
      }
      else if (command.startsWith("speed"))
      {
        motorSpeed = command.substring(5).toInt();
      }
      break;
    }
  }
}

/**
 * Sets up the async web server, attaching websocket handlers for motor control.
 */
void setupMotorServer()
{
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, NULL);
  WiFi.disconnect();

  // Initialize the motor shield
  if (!AFMS.begin())
  {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }

  // Server routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", HTML_INDEX); });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/css", CSS_STYLE); });

  server.on("/control.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "application/javascript", JS_CONTROL); });

  server.begin();
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
