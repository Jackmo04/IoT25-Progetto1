#include "include/kernel.h"
#include "include/config.h"
#include "include/core.h"
#include "include/input.h"
#include "include/leds.h"
#include "include/display.h"

void setup() {
  initCore();
  initInput();
  initLeds();
  initDisplay();
  changeState(INTRO_STATE);
}

void loop(){ 
  updateStateTime(); 
  switch (getCurrentState()) { 
  case INTRO_STATE:
    intro();
    break;
  case STAGE1_STATE:
    stage1();
    break;
  case STAGE2_STATE:
    stage2();
    break;
  case FINAL_STATE:
    finalize();
    break;
  }
}
