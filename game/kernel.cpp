#include "include/kernel.h"

int currentState;
long enteredStateTime;
long currentTimeInState;
bool firstCheckInState; 

int getCurrentState(){
  return currentState;
}

long getCurrentTimeInState(){
  return currentTimeInState;
}

long getEnteredStateTime(){
  return enteredStateTime;
}

bool isJustEnteredInState(){
  bool com = firstCheckInState;
  firstCheckInState = false;
  return com;
}

void changeState(int newState){
  currentState = newState;
  enteredStateTime = millis();
  firstCheckInState = true;
}

void updateStateTime(){
  currentTimeInState = millis() - enteredStateTime;
}
