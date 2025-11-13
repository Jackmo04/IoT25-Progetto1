#ifndef __KERNEL__
#define __KERNEL__

int getCurrentState();
void changeState(int newState);
void updateStateTime();
void resetStateTime();
long getCurrentTimeInState();
long getEnteredStateTime();
bool isJustEnteredInState();

#endif
