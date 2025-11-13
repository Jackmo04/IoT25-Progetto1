#ifndef __BUTTONS__
#define __BUTTONS__

void buttonHandler(int i, unsigned long timestamp);
void initButtons();
bool isButtonPressed(int buttonIndex);
void prepareSleep();

#endif
