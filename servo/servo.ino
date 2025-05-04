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

    if (client) {
        Serial.println("New Client.");
        String currentLine = "";

        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);

                if (c == '\n') {
                    if (currentLine.startsWith("GET /")) {
                        handleCommand(currentLine.substring(5, 6));
                        client.println("HTTP/1.1 200 OK");
                        client.println("Access-Control-Allow-Origin: *");
                        client.println("Content-type:text/html");
                        client.println();
                        client.print("Servo angle: ");
                        client.print(angle);
                        client.println();
                        Serial.println(currentLine);
                        Serial.print("Servo angle: ");
                        Serial.println(angle);
                        break;
                    }

                    currentLine = "";
                    break;
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }

        client.stop();
        Serial.println("Client Disconnected.");
    }

    angle += scan;
                        
    if (angle < 10) {
        angle = 10;
        scan = 1;
    } else if (angle > 170) {
        angle = 170;
        scan = -1;
    }

    servo.write(angle);
    delay(50);
}

void handleCommand(String command) {
    if (command == "L") {
        scan = -1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Person");
        lcd.setCursor(0, 1);
        lcd.print("detected!");
    } else if (command == "R") {
        scan = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Person");
        lcd.setCursor(0, 1);
        lcd.print("detected!");
    } else if (command == "F") {
        scan = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Person");
        lcd.setCursor(0, 1);
        lcd.print("detected!");
    } else if (command == "S") {
        if (scan == 0) {
            scan = 1;
        }
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Scanning...");
    } else if (command == "T") {
        tone(signalPin, 1000);
    } else if (command == "N") {
        noTone(signalPin);
    }
}