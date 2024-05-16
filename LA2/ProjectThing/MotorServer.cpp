#include "ProjectThing.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *L_MOTOR = AFMS.getMotor(3);
Adafruit_DCMotor *R_MOTOR = AFMS.getMotor(4);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char *ssid = "RoboCar (visit 192.168.4.1)";
int numClients = 0;
int motorSpeed = 128;

int forward(String command)
{
  R_MOTOR->setSpeed(motorSpeed);
  R_MOTOR->run(FORWARD);
  L_MOTOR->setSpeed(motorSpeed);
  L_MOTOR->run(FORWARD);

  return 1;
}

int backward(String command)
{
  R_MOTOR->setSpeed(motorSpeed);
  R_MOTOR->run(BACKWARD);
  L_MOTOR->setSpeed(motorSpeed);
  L_MOTOR->run(BACKWARD);

  return 1;
}

int left(String command)
{
  R_MOTOR->setSpeed(motorSpeed);
  R_MOTOR->run(FORWARD);
  L_MOTOR->setSpeed(motorSpeed);
  L_MOTOR->run(BACKWARD);

  return 1;
}

int right(String command)
{
  R_MOTOR->setSpeed(motorSpeed);
  R_MOTOR->run(BACKWARD);
  L_MOTOR->setSpeed(motorSpeed);
  L_MOTOR->run(FORWARD);

  return 1;
}

int stop(String command)
{
  R_MOTOR->setSpeed(0);
  R_MOTOR->run(RELEASE);
  L_MOTOR->setSpeed(0);
  L_MOTOR->run(RELEASE);

  return 1;
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
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

  case WS_EVT_DISCONNECT:
    Serial.printf("[%u] Disconnected!\n", client->id());
    if (numClients > 0)
    {
      numClients--;
    }

    break;

  case WS_EVT_DATA:
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len)
    {
      // the whole message is in a single frame and we got all of it's data
      char *msg = new char[len + 1];
      memcpy(msg, data, len);
      msg[len] = '\0';
      String command = msg;
      delete[] msg;

      Serial.println(command);

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

void setupMotorServer()
{
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, NULL);
  WiFi.disconnect();

  if (!AFMS.begin())
  {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }

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
