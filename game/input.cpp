#include "Arduino.h"
#include "include/input.h"
#include "include/config.h"

#include <EnableInterrupt.h>

#define BOUNCING_TIME 250

uint8_t inputPins[NUM_BUTTONS] = {BUT01_PIN, BUT02_PIN, BUT03_PIN, BUT04_PIN};
bool buttonPressed[NUM_BUTTONS] = {false, false, false, false};
long lastButtonPressedTimestamps[NUM_BUTTONS];

bool sleepMode = false;

void buttonHandler(int i, long timestamp);
void buttonHandler0(){ buttonHandler(0, millis()); }
void buttonHandler1(){ buttonHandler(1, millis()); }
void buttonHandler2(){ buttonHandler(2, millis()); }
void buttonHandler3(){ buttonHandler(3, millis()); }

void (*buttonHandlers[NUM_BUTTONS])() = { buttonHandler0, buttonHandler1, buttonHandler2, buttonHandler3 };

void buttonHandler(int i, long timestamp){
  if (sleepMode) {
    sleepMode = false;
    return;
  }
  if (timestamp - lastButtonPressedTimestamps[i] > BOUNCING_TIME) {
    int status = digitalRead(inputPins[i]);
    if (status == HIGH && !buttonPressed[i]) {
        buttonPressed[i] = true;
        lastButtonPressedTimestamps[i] = timestamp;
    }
  } else {
    Serial.println("Button " + String(i+1) + " ignored due to bouncing");
  }
}

void initInput(){
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(inputPins[i], INPUT);  
    enableInterrupt(inputPins[i], buttonHandlers[i], CHANGE);
  }
}

void resetInput(){
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttonPressed[i] = false;
    lastButtonPressedTimestamps[i] = millis();
  }
}

void resetButtons(){
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttonPressed[i] = false;
  }
}

bool isButtonPressed(int buttonIndex){
  return buttonPressed[buttonIndex];
}

void prepareSleep(){
  sleepMode = true;
}
