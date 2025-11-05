#ifndef __LEDS__
#define __LEDS__

void initLeds();
void setGreenLed(int number, bool on);
void setRedLed(bool on);
void pulseRedLedInIntro();
void allLedsOff();

#endif
