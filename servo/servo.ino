#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <../config.h>

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
NetworkServer server(80);
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int angle = 90;
int scan = 3;
int signalPin = 4;
int toneDuration = 0;

void setup() {
  Serial.begin(115200);
  servo.attach(15);
  pinMode(signalPin, OUTPUT);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IP address: ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  server.begin();
}

void loop() {
  NetworkClient client = server.accept();

  if (toneDuration > 0) {
    if (toneDuration > 50) {
      tone(signalPin, 1000);
    } else {
      noTone(signalPin);
    }
    
    toneDuration--;
  } else {
    noTone(signalPin);
  }

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

        if (currentLine.startsWith("GET /")) {
          handleCommand(currentLine.substring(5));
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

  delay(10);
}

void handleCommand(String command) {
  lcd.clear();
  lcd.setCursor(0, 0);
  char commandDirection = command.charAt(0);
  char commandTone = command.charAt(1);
  Serial.print("commandDirection: ");
  Serial.println(commandDirection);
  Serial.print("commandTone: ");
  Serial.println(commandTone);

  switch (commandDirection) {
    case 'L':
      angle = max(angle - 5, 10);
      break;
    case 'R':
      angle = min(angle + 5, 170);
      break;
    case 'S':
      angle += scan;
      toneDuration = 0;

      if (angle < 10) {
        angle = 10;
        scan = 3;
      } else if (angle > 170) {
        angle = 170;
        scan = -3;
      }

      break;
  }

  switch (commandDirection) {
    case 'L':
    case 'R':
    case 'F':
      lcd.print("Person");
      lcd.setCursor(0, 1);
      lcd.print("detected!");
      break;
    case 'S':
      lcd.print("Scanning...");
  }

  switch (commandTone) {
    case 'T':
      if (toneDuration < 1) {
        toneDuration = 100;
      }

      break;
    case 'N':
      toneDuration = 0;
      break;
  }
}