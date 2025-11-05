#ifndef __DISPLAY__
#define __DISPLAY__

void initDisplay();
void displayWelcome();
void displayGo();
void displaySequence(int seq[4]);
void displaySuccess(int score);
void displayGameOver(int score);
void displayScore(int score);
void lcdSleep();
void lcdWake();
void displayLevel(int level, unsigned long startingTime);
void gameOver();

#endif
