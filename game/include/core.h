#ifndef __CORE__
#define __CORE__

#define INTRO_STATE     1
#define SLEEPING_STATE  2
#define GAME_STATE      3

void initCore();
void intro();
void prepareGame();
void playGame();
void sleepNow();

#endif
