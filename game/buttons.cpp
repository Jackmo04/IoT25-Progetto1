#include "include/buttons.h"
#include "include/config.h"

#include <Arduino.h>
#include <EnableInterrupt.h>

#define BOUNCING_TIME 250

unsigned short inputPins[NUM_BUTTONS] = {BTN_01_PIN, BTN_02_PIN, BTN_03_PIN, BTN_04_PIN};
volatile bool buttonPressed[NUM_BUTTONS] = {false, false, false, false};
volatile int pressOrder[NUM_BUTTONS] = {0, 0, 0, 0};
volatile int pressCount = 0;
unsigned long lastButtonPressedTimestamps[NUM_BUTTONS];

void buttonHandler0() { buttonHandler(0); }
void buttonHandler1() { buttonHandler(1); }
void buttonHandler2() { buttonHandler(2); }
void buttonHandler3() { buttonHandler(3); }

void (*buttonHandlers[NUM_BUTTONS])() = {buttonHandler0, buttonHandler1, buttonHandler2, buttonHandler3};

void buttonHandler(int i)
{
  unsigned long timestamp = millis();
  if (timestamp - lastButtonPressedTimestamps[i] > BOUNCING_TIME)
  {
    int status = digitalRead(inputPins[i]);
    if (status == HIGH && pressCount < NUM_BUTTONS)
    {
      buttonPressed[i] = true;
      pressOrder[pressCount] = i + 1;
      pressCount++;
      lastButtonPressedTimestamps[i] = timestamp;
    }
  }
#ifdef DEBUG_VERBOSE
  else
  {
    Serial.println("Button " + String(i + 1) + " ignored due to bouncing");
  }
#endif
}

void initButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    pinMode(inputPins[i], INPUT);
    enableInterrupt(inputPins[i], buttonHandlers[i], CHANGE);
    lastButtonPressedTimestamps[i] = millis();
    buttonPressed[i] = false;
    pressOrder[i] = 0;
  }
  pressCount = 0;
}

bool isButtonPressed(int buttonIndex)
{
  return buttonPressed[buttonIndex];
}

void resetButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    buttonPressed[i] = false;
    pressOrder[i] = 0;
  }
  pressCount = 0;
}

int* getPressOrder()
{
  return pressOrder;
}

void wakeUp()
{
#ifdef WOKWI_SIMULATION
  buttonPressed[0] = true;
#endif
}

void prepareSleep()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    disableInterrupt(inputPins[i]);
  }
  enableInterrupt(inputPins[0], wakeUp, CHANGE);
}

void endSleep()
{
  disableInterrupt(inputPins[0]);
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    enableInterrupt(inputPins[i], buttonHandlers[i], CHANGE);
    buttonPressed[i] = false;
  }
}
