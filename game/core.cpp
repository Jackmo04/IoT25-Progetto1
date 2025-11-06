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
#define GAMEOVER_LED_DURATION 2000
#define GAMEOVER_TOTAL_DURATION 10000
#define STARTING_TIME 10000

unsigned long T1_initial = 10000;
unsigned long currentT1;
unsigned long roundStartT = 0;

float F_level = 0.92; //da sistemare

int currentSequence[4];
int inputPos = 0;
int score = 0;
bool inGame = false;
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
  displayWelcome();
  resetButtons();
}

void intro(){
  if (isJustEnteredInState()){
    #ifdef DEBUG
    Serial.println("Intro...");
    #endif
    introSetup();
  }

  pulseRedLedInIntro();
  int lvl;
  if ((lvl = readLevelFromPot()) != level){
    level = lvl;
    displayLevel(level, millis());
    #ifdef DEBUG
    Serial.print("Level changed to: "); Serial.println(level);
    #endif
  }

  if (isButtonPressed(0)){
    changeState(PREPARE_STATE);
    return;
  }

  if (getCurrentTimeInState() > MAX_TIME_BEFORE_SLEEP){
    sleepNow();
    changeState(INTRO_STATE);
  }
}

void prepareGame(){
  if (isJustEnteredInState()){
    #ifdef DEBUG
    Serial.println("Preparing the game...");
    #endif
    allLedsOff();
    resetButtons();
    score = 0;
    inGame = true;
    currentT1 = T1_initial;
    inputPos = 0;
    //da modificare
    int lvl = readLevelFromPot();
    F_level = factorForLevel(lvl);
    displayGo();
    delay(1000);
    changeState(GAME_STATE);
    return;
  }
}

void playGame(){
  static bool roundActive = false;
  if (isJustEnteredInState()){
    #ifdef DEBUG
    Serial.println("Starting game...");
    #endif
    roundActive = false;
    resetButtons();
  }

  // Start of every round
  if (!roundActive) {
    allLedsOff();
    generateSequence(currentSequence);
    displaySequence(currentSequence);
    roundStartT = millis();
    inputPos = 0;
    #ifdef DEBUG
    Serial.print("Sequence: ");
    for (int i = 0; i < 4; i++) 
      Serial.print(currentSequence[i]);
    Serial.println();
    #endif
    roundActive = true;
  }

  if (roundActive && (millis() - roundStartT) > currentT1){
    #ifdef DEBUG
    Serial.println("Timeout - Game Over");
    #endif
    gameOver();
    return;
  }

  for (int b = 0; b < NUM_BUTTONS; b++){
    if (isButtonPressed(b)) {
      int pressedDigit = b + 1;
      #ifdef DEBUG
      Serial.println("Pressed: " + String(pressedDigit));
      #endif
      setGreenLed(pressedDigit, true);
      if (pressedDigit == currentSequence[inputPos]) {
        inputPos++;
        if (inputPos >= 4){
          score++;
          displaySuccess(score);
          #ifdef DEBUG
          Serial.println("Correct! Score: " + String(score));
          #endif
          currentT1 = (unsigned long)((float)currentT1 * F_level);
          if (currentT1 < 500) 
            currentT1 = 500; //viene posto come tempo minimo di durata del round
          roundActive = false;
          delay(1500);
        }
      } else {
        #ifdef DEBUG
        Serial.println("Wrong button!");
        #endif
        gameOver();
        return;
      }
      resetButtons();
    }
  }
}

void gameOver(){
  #ifdef DEBUG
  Serial.println("Game Over! Final Score: " + String(score));
  Serial.println("Returning to Intro in " + String(GAMEOVER_TOTAL_DURATION / 1000.0) + " seconds...");
  #endif
  allLedsOff();
  setRedLed(true);
  displayGameOver(score);
  delay(GAMEOVER_LED_DURATION);
  setRedLed(false);
  delay(GAMEOVER_TOTAL_DURATION - GAMEOVER_LED_DURATION);
  changeState(INTRO_STATE);
}

void sleepNow() {
  allLedsOff();
  lcdSleep();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  #ifdef DEBUG
  Serial.println("Going to sleep...");
  #endif
  prepareSleep();
  sleep_mode();
  sleep_disable();
  initButtons();
  #ifdef DEBUG
  Serial.println("Woke up!");
  #endif
  lcdWake();
}
