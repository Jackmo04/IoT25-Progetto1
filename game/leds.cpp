#include "Arduino.h"
#include "include/leds.h"
#include "include/config.h"

static int greenPins[4] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};

unsigned long lastPulseT = 0;
int pulseDir = 1;
int pulseVal = 0;

void initLeds(){
  for (int i = 0; i < 4; i++) pinMode(greenPins[i], OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  allLedsOff();
}

void setLed(int index, bool on){
  digitalWrite(greenPins[index-1], on ? HIGH : LOW);
}

void setRedLed(bool on){
  digitalWrite(RED_LED_PIN, on ? HIGH : LOW);
}

void allLedsOff(){
  for (int i = 1; i <= 4; i++) setLed(i, false);
  setRedLed(false);
}

void pulseRedLedInIntro(){
  unsigned long now = millis();
  if (now - lastPulseT < 20) return;
  lastPulseT = now;
  pulseVal += pulseDir * 8;
  if (pulseVal >= 255) { pulseVal = 255; pulseDir = -1; }
  else if (pulseVal <= 0) { pulseVal = 0; pulseDir = 1; }
  analogWrite(RED_LED_PIN, pulseVal);
}
