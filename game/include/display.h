#ifndef __DISPLAY__
#define __DISPLAY__

#include "config.h"

void initDisplay();
void displayWelcome();
void displayLevel(int level);
void displayGo();
void displaySequence(int seq[NUM_BUTTONS]);
void displaySuccess(int score);
void displayGameOver(int score);
void displayScore(int score);
void lcdSleep();
void lcdWake();

#endif
