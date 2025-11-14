#include "include/leds.h"
#include "include/config.h"

#include <Arduino.h>

#define PULSE_SPEED_FACTOR 5
#define PULSE_INTERVAL 20

static int greenPins[NUM_BUTTONS] = {LED_1_PIN, LED_2_PIN, LED_3_PIN, LED_4_PIN};

unsigned long lastPulseT = 0;
int pulseDir = 1;
int pulseVal = 0;

void initLeds()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(greenPins[i], OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  allLedsOff();
}

void setGreenLed(int number, bool on)
{
  digitalWrite(greenPins[number - 1], on ? HIGH : LOW);
}

void setRedLed(bool on)
{
  digitalWrite(RED_LED_PIN, on ? HIGH : LOW);
}

void allLedsOff()
{
  for (int i = 1; i <= NUM_BUTTONS; i++)
    setGreenLed(i, false);
  setRedLed(false);
}

void pulseRedLedInIntro()
{
  unsigned long now = millis();
  if (now - lastPulseT < PULSE_INTERVAL)
    return;
  lastPulseT = now;
  pulseVal += pulseDir * PULSE_SPEED_FACTOR;
  if (pulseVal >= 255)
  {
    pulseVal = 255;
    pulseDir = -1;
  }
  else if (pulseVal <= 0)
  {
    pulseVal = 0;
    pulseDir = 1;
  }
  analogWrite(RED_LED_PIN, pulseVal);
}
