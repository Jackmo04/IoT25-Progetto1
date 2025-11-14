#ifndef __BUTTONS__
#define __BUTTONS__

void buttonHandler(int i);
void initButtons();
bool isButtonPressed(int buttonIndex);
int* getPressOrder();
void resetButtons();
void prepareSleep();
void endSleep();
void wakeUp();

#endif
