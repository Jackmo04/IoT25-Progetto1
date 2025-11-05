#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NUM 4

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int BUTTONS[NUM] = {2, 3, 4, 5};
const int LEDS[NUM] = {8, 9, 10, 11};
const int POT = A0;
const int RED_LED = 13;

void setup()
{
  for (int i = 0; i < NUM; i++) {
    pinMode(LEDS[i], OUTPUT);
    pinMode(BUTTONS[i], INPUT);
  }
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Harduro e' hard");
}

void loop()
{
  for (int i = 0; i < NUM; i++) {
    if (digitalRead(BUTTONS[i]) == HIGH) {
      digitalWrite(LEDS[i], HIGH);
      Serial.println(String("Premuto ") + i);
    }
  }
  int p = analogRead(POT);
  int p1 = map(p, 0, 1024, 1, 5);
  lcd.setCursor(0,1);
  lcd.print(p1);
  Serial.println(String("POTENZA: ") + p1);
}
