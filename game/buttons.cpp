#include "include/buttons.h"
#include "include/config.h"

#include <Arduino.h>
#include <EnableInterrupt.h>

#define BOUNCING_TIME 250

unsigned short inputPins[NUM_BUTTONS] = {BTN_01_PIN, BTN_02_PIN, BTN_03_PIN, BTN_04_PIN};
bool buttonPressed[NUM_BUTTONS] = {false, false, false, false};
unsigned long lastButtonPressedTimestamps[NUM_BUTTONS];
bool sleeping = false;

void buttonHandler0() { buttonHandler(0, millis()); }
void buttonHandler1() { buttonHandler(1, millis()); }
void buttonHandler2() { buttonHandler(2, millis()); }
void buttonHandler3() { buttonHandler(3, millis()); }

void (*buttonHandlers[NUM_BUTTONS])() = {buttonHandler0, buttonHandler1, buttonHandler2, buttonHandler3};

void buttonHandler(int i, unsigned long timestamp)
{
  if (sleeping)
  {
#ifdef WOKWI_SIMULATION
    buttonPressed[i] = true;
#endif
    sleeping = false;
    return;
  }
  if (timestamp - lastButtonPressedTimestamps[i] > BOUNCING_TIME)
  {
    int status = digitalRead(inputPins[i]);
    if (status == HIGH && !buttonPressed[i])
    {
      buttonPressed[i] = true;
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
  }
}

bool isButtonPressed(int buttonIndex)
{
  bool pressed = buttonPressed[buttonIndex];
  buttonPressed[buttonIndex] = false;
  return pressed;
}

void prepareSleep()
{
  for (int i = 1; i < NUM_BUTTONS; i++)
  {
    disableInterrupt(inputPins[i]);
  }
  sleeping = true;
}
