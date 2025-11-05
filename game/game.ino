#include "kernel.h"
#include "config.h"
#include "core.h"
#include "input.h"
#include "leds.h"
#include "display.h"

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
