#include <ESP32Servo.h>
#include <WiFi.h>
#include <../config.h>

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
NetworkServer server(80);
Servo servo;
int angle = 90;
int scan = 3;
int signalPin = 4;

void setup() {
  Serial.begin(115200);
  servo.attach(15);
  pinMode(signalPin, OUTPUT);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  NetworkClient client = server.accept();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n') {
          currentLine = "";
          break;
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /L") || currentLine.endsWith("GET /R") || currentLine.endsWith("GET /S")) {
          if (currentLine.endsWith("GET /L")) {
            digitalWrite(signalPin, HIGH);
            angle -= 5;

            if (angle < 10) {
              angle = 10;
            }
          } else if (currentLine.endsWith("GET /R")) {
            digitalWrite(signalPin, HIGH);
            angle += 5;

            if (angle > 170) {
              angle = 170;
            }
          } else if (currentLine.endsWith("GET /S")) {
            digitalWrite(signalPin, LOW);
            angle += scan;

            if (angle < 10) {
              angle = 10;
              scan = 3;
            } else if (angle > 170) {
              angle = 170;
              scan = -3;
            }
          }

          servo.write(angle);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.print("Servo angle: ");
          client.print(angle);
          client.println();
          break;
        }
      }
    }
    
    client.stop();
    Serial.println("Client Disconnected.");
  }
}