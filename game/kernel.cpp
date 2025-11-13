#include "include/kernel.h"

#include <Arduino.h>

int currentState;
long enteredStateTime;
long currentTimeInState;
bool firstCheckInState;

int getCurrentState()
{
  return currentState;
}

long getCurrentTimeInState()
{
  return currentTimeInState;
}

long getEnteredStateTime()
{
  return enteredStateTime;
}

bool isJustEnteredInState()
{
  bool temp = firstCheckInState;
  firstCheckInState = false;
  return temp;
}

void changeState(int newState)
{
  currentState = newState;
  enteredStateTime = millis();
  firstCheckInState = true;
}

void updateStateTime()
{
  currentTimeInState = millis() - enteredStateTime;
}

void resetStateTime()
{
  enteredStateTime = millis();
  currentTimeInState = 0;
}
