#ifndef __KERNEL__
#define __KERNEL__

#include "Arduino.h" 

void initKernel();
void updateStateTime();
void changeState(int newState);
void resetStateTime();

int getCurrentState();
long getCurrentTimeInState();
long getEnteredStateTime();
bool isJustEnteredInState(); 

#endif
