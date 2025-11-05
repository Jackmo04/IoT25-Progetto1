#include "Arduino.h"
#include "include/core.h"
#include "include/kernel.h"
#include "include/buttons.h"
#include "include/leds.h"
#include "include/pot.h"
#include "include/display.h"
#include "include/config.h"

#include <avr/sleep.h>

#define MAX_TIME_BEFORE_SLEEP 10000
#define GAMEOVER_RED_DURATION 2000
#define SHOW_GAMEOVER_SECONDS 10000

unsigned long T1_initial = 10000;
unsigned long currentT1;

float F_level = 0.92; //da sistemare

int currentSequence[4];
int inputPos = 0;
int score = 0;
bool inGame = false;
unsigned long roundStartT = 0;
int level = 1;

void initCore(){
  randomSeed(analogRead(A7));
}

float factorForLevel(int lvl){ // TODO numeri casuali da sistemare
  switch(lvl){
    case 1: return 0.92;
    case 2: return 0.86;
    case 3: return 0.79;
    case 4: return 0.70;
  }
  return 0.92;
}

void generateSequence(int seq[4]){
  int n[4] = {1, 2, 3, 4};
  for (int i = 0; i < 4; i++){
    int r = random(i, 4);
    int t = n[i];
    n[i] = n[r];
    n[r] = t;
  }
  for (int i = 0; i < 4; i++) seq[i] = n[i];
}

void introSetup(){
  allLedsOff();
  showWelcome();
  resetButtons();
}

void intro(){
  if (isJustEnteredInState()){
    Serial.println("Intro...");
    introSetup();
  }

  pulseRedLedInIntro();
  int lvl;
  if ((lvl = readLevelFromPot()) != level){
    level = lvl;
    showLevel(level, millis());
    Serial.print("Level changed to: "); Serial.println(level);
  }

  if (isButtonPressed(0)){
    changeState(STAGE1_STATE);
    return;
  }

  if (getCurrentTimeInState() > MAX_TIME_BEFORE_SLEEP){
    sleepNow();
    changeState(INTRO_STATE);
  }
}

void stage1(){
  if (isJustEnteredInState()){
    Serial.println("Stage1... starting game");
    allLedsOff();
    showGo();
    resetButtons();
    score = 0;
    inGame = true;
    currentT1 = T1_initial;
    inputPos = 0;
    //da modificare
    int lvl = readLevelFromPot();
    F_level = factorForLevel(lvl);
    delay(800);
    changeState(STAGE2_STATE);
    return;
  }
}

void stage2(){
  static bool roundActive = false;
  if (isJustEnteredInState()){
    Serial.println("Stage2...");
    roundActive = false;
    resetButtons();
  }

  if (!roundActive){
    allLedsOff();
    generateSequence(currentSequence);
    showSequence(currentSequence);
    roundStartT = millis();
    inputPos = 0;
    roundActive = true;
    Serial.print("Seq: ");
    for (int i=0;i<4;i++) Serial.print(currentSequence[i]);
    Serial.println();
  }

  if (roundActive && (millis() - roundStartT) > currentT1){
    Serial.println("Timeout - Game Over");
    gameOver();
    return;
  }

  for (int b = 0; b < NUM_BUTTONS; b++){
    if (isButtonPressed(b)) {
      int pressedDigit = b + 1;
      Serial.println("Pressed: " + String(pressedDigit));
      setGreenLed(pressedDigit, true);
      if (pressedDigit == currentSequence[inputPos]) {
        inputPos++;
        if (inputPos >= 4){
          score++;
          showGood(score);
          Serial.print("Good! score=");
          Serial.println(score);
          currentT1 = (unsigned long)((float)currentT1 * F_level);
          if (currentT1 < 500) 
            currentT1 = 500; //viene posto come tempo minimo di durata del round
          delay(800);
          roundActive = false;
          showScore(score);
          delay(300);
        }
      } else {
        Serial.println("Wrong button - Game Over");
        gameOver();
        return;
      }
      resetButtons();
    }
  }
}

void gameOver(){
  allLedsOff();
  setRedLed(true);
  showGameOver(score);
  unsigned long t = millis();
  while (millis() - t < GAMEOVER_RED_DURATION){
    delay(50);
  }
  setRedLed(false);
  unsigned long t2 = millis();
  while (millis() - t2 < SHOW_GAMEOVER_SECONDS){
    delay(50);
  }
  changeState(INTRO_STATE);
}

//inutile
void finalize(){
  if (isJustEnteredInState()){
    Serial.println("Finalize...");
  }
  changeState(INTRO_STATE);
}

void sleepNow() {
  allLedsOff();
  lcdSleep();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  Serial.println("Going to sleep...");
  prepareSleep();
  sleep_mode();
  sleep_disable();
  initButtons();
  Serial.println("Woke up!");
  lcdWake();
}
