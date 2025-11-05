#include "core.h"
#include "Arduino.h"
#include "kernel.h"
#include "input.h"
#include "leds.h"
#include "display.h"
#include "config.h"

#include <avr/sleep.h>

#define MAX_TIME_IN_INTRO_STATE 10000
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
  Serial.begin(9600);
  randomSeed(analogRead(A7));
  initLeds();
  initDisplay();
}

int readLevelFromPot(){
  int v = analogRead(POT_PIN);
  int level = map(v, 0, 1023, 1, 4);
  Serial.println("Level: " + String(level));
  return level;
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

void enterInitialStateSetup(){
  allLedsOff();
  showWelcome();
}

void intro(){
  if (isJustEnteredInState()){
    Serial.println("Intro...");
    enterInitialStateSetup();
    resetInput();
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

  if (getCurrentTimeInState() > MAX_TIME_IN_INTRO_STATE){
    allLedsOff();
    digitalWrite(RED_LED_PIN, LOW);
    sleepNow();
    changeState(INTRO_STATE);
  }
}

void lcdDisplaySleep(){
  clearDisplay();
}

void stage1(){
  if (isJustEnteredInState()){
    Serial.println("Stage1... starting game");
    allLedsOff();
    showGo();
    resetInput();
    score = 0;
    inGame = true;
    currentT1 = T1_initial;
    inputPos = 0;
    //da modificare
    int lvl = readLevelFromPot();
    F_level = factorForLevel(lvl);
    Serial.print("Level: "); Serial.println(lvl);
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
    resetInput();
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
      setLed(pressedDigit, true);

      if (pressedDigit == currentSequence[inputPos]) {
        inputPos++;
        if (inputPos >= 4){
          score++;
          showGood(score);
          Serial.print("Good! score=");
          Serial.println(score);
          currentT1 = (unsigned long)((float)currentT1 * F_level);
          if (currentT1 < 500) currentT1 = 500; //viene posto come tempo minimo di durata del round
          delay(800);
          resetButtons();
          roundActive = false;
          showScore(score);
          delay(300);
        } else {
          resetButtons();
        }
      } else {
        Serial.println("Wrong button - Game Over");
        resetInput();
        gameOver();
        return;
      }
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

void wakeUpNow() {
}

void sleepNow() {
  lcdDisplaySleep();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  prepareSleep();
  Serial.println("Going to sleep...");
  sleep_mode();
  Serial.println("Woke up!");
  sleep_disable();
}
