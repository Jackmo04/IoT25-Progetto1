#include "include/kernel.h"
#include "include/config.h"
#include "include/core.h"
#include "include/buttons.h"
#include "include/leds.h"
#include "include/display.h"

#include <Arduino.h>

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
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
    case SLEEPING_STATE:
      sleepNow();
      break;
    case GAME_STATE:
      playGame();
      break;
  }
}
