#ifndef __LEDS__
#define __LEDS__

void initLeds();
void setLed(int index, bool on);
void setRedLed(bool on);
void pulseRedLedInIntro();
void allLedsOff();

#endif
