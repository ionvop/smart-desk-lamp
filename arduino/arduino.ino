#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int signalPin = 7;
int buzzerPin = 5;

void setup() {
  pinMode(signalPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  int signal = digitalRead(signalPin);
  Serial.println(signal);
  lcd.clear();

  if (signal == HIGH) {
    lcd.setCursor(0, 0);
    lcd.print("Intruder alert");
    tone(buzzerPin, 1000);
    delay(500);
    noTone(buzzerPin);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Scanning....");
  }

  delay(500);
}
