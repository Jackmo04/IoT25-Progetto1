#include "include/kernel.h"
#include "include/config.h"
#include "include/core.h"
#include "include/buttons.h"
#include "include/leds.h"
#include "include/display.h"

void setup() {
  Serial.begin(9600);
  initButtons();
  initLeds();
  initDisplay();
  initCore();
  changeState(INTRO_STATE);
}

void loop(){ 
  updateStateTime(); 
  switch (getCurrentState()) { 
  case INTRO_STATE:
    intro();
    break;
  case PREPARE_STATE:
    prepareGame();
    break;
  case GAME_STATE:
    playGame();
    break;
  }
}
