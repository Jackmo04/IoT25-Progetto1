#ifndef __CORE__
#define __CORE__

#include "config.h"

#define INTRO_STATE 1
#define SLEEPING_STATE 2
#define GAME_STATE 3

void initCore();
double getLevelFactor();
void generateSequence(int seq[NUM_BUTTONS]);
void intro();
void prepareGame();
void prepareRound();
void playGame();
void gameOver();
void sleepNow();

#endif
