#ifndef __DISPLAY__
#define __DISPLAY__

void initDisplay();
void showWelcome();
void showGo();
void showSequence(int seq[4]);
void showGood(int score);
void showGameOver(int score);
void showScore(int score);
void clearDisplay();
void lcdDisplaySleep();
void gameOver();

#endif
