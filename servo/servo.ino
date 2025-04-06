#include <ESP32Servo.h>
#include <WiFi.h>
#include <../config.h>

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
NetworkServer server(80);
Servo servoH;
Servo servoV;
int angleH = 90;
int angleV = 90;

void setup() {
  Serial.begin(115200);
  servoH.attach(16);
  servoV.attach(17);
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

        if (currentLine.endsWith("GET /L") || currentLine.endsWith("GET /R") || currentLine.endsWith("GET /U") || currentLine.endsWith("GET /D")) {
          if (currentLine.endsWith("GET /L")) {
            angleH -= 10;

            if (angleH < 10) {
              angleH = 10;
            }
          } else if (currentLine.endsWith("GET /R")) {
            angleH += 10;

            if (angleH > 170) {
              angleH = 170;
            }
          } else if (currentLine.endsWith("GET /U")) {
            angleV += 10;

            if (angleV > 170) {
              angleV = 170;
            }
          } else if (currentLine.endsWith("GET /D")) {
            angleV -= 10;

            if (angleV < 10) {
              angleV = 10;
            }
          }

          servoH.write(angleH);
          servoV.write(angleV);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.print("Servo angleH: ");
          client.print(angleH);
          client.print(", Servo angleV: ");
          client.println(angleV);
          client.println();
          break;
        }
      }
    }
    
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
