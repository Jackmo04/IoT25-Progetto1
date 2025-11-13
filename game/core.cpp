#include "include/core.h"
#include "include/kernel.h"
#include "include/buttons.h"
#include "include/leds.h"
#include "include/pot.h"
#include "include/display.h"
#include "include/config.h"

#include <Arduino.h>
#include <avr/sleep.h>

#define MAX_TIME_BEFORE_SLEEP 10000
#define GAMEOVER_LED_DURATION 2000
#define GAMEOVER_TOTAL_DURATION 10000
#define LVL_SCREEN_DURATION 2000
#define GO_SCREEN_DURATION 1000
#define INTER_ROUND_DELAY 1500

unsigned long availableTime = STARTING_AVAILABLE_TIME;
unsigned long roundStartT = 0;

int currentSequence[4];
int currentSeqIndex = 0;
int score = 0;
bool isRoundActive = false;
bool inGame = false;

int level = 1;
double levelfactor;

void initCore()
{
  randomSeed(analogRead(A7));
}

double getLevelFactor()
{
  switch (level)
  {
  case 1:
    return LEVEL_1_FACTOR;
  case 2:
    return LEVEL_2_FACTOR;
  case 3:
    return LEVEL_3_FACTOR;
  case 4:
    return LEVEL_4_FACTOR;
  default:
    return LEVEL_1_FACTOR;
  }
}

void generateSequence(int seq[NUM_BUTTONS])
{
  int n[NUM_BUTTONS] = {1, 2, 3, 4};
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    int r = random(i, NUM_BUTTONS);
    int t = n[i];
    n[i] = n[r];
    n[r] = t;
  }
  for (int i = 0; i < NUM_BUTTONS; i++)
    seq[i] = n[i];
}

void intro()
{
  if (isJustEnteredInState())
  {
#ifdef DEBUG
    Serial.println("Intro...");
#endif
    allLedsOff();
    displayWelcome();
  }
  pulseRedLedInIntro();

  int lvl;
  bool levelChanged = false;
  
  if ((lvl = readLevelFromPot()) != level)
  {
    unsigned long lastLvlChangeT = millis();
    levelChanged = true;
    while (millis() - lastLvlChangeT <= LVL_SCREEN_DURATION)
    {
      if ((lvl = readLevelFromPot()) != level)
      {
        level = lvl;
        lastLvlChangeT = millis();
        displayLevel(level);
#ifdef DEBUG
        Serial.print("Level changed to: ");
        Serial.println(level);
#endif
      }
      pulseRedLedInIntro();
    }
  }

  if (levelChanged)
  {
    displayWelcome();
    resetStateTime();
    levelChanged = false;
  }

  if (isButtonPressed(0))
  {
    changeState(GAME_STATE);
    return;
  }

  if (getCurrentTimeInState() > MAX_TIME_BEFORE_SLEEP)
  {
    changeState(SLEEPING_STATE);
  }
}

void prepareGame()
{
#ifdef DEBUG
  Serial.println("Preparing the game...");
#endif
  allLedsOff();
  score = 0;
  inGame = true;
  availableTime = STARTING_AVAILABLE_TIME;
  isRoundActive = false;
  // da modificare
  levelfactor = getLevelFactor();
  displayGo();
  delay(GO_SCREEN_DURATION);
}

void prepareRound()
{
  allLedsOff();
  generateSequence(currentSequence);
  displaySequence(currentSequence);
#ifdef DEBUG
  Serial.print("Sequence: ");
  for (int i = 0; i < 4; i++)
    Serial.print(currentSequence[i]);
  Serial.println();
#endif
  currentSeqIndex = 0;
  isRoundActive = true;
  roundStartT = millis();
}

void playGame()
{
  if (isJustEnteredInState())
  {
    prepareGame();
#ifdef DEBUG
    Serial.println("Starting game...");
#endif
  }

  if (!isRoundActive)
  {
    prepareRound();
  }

  if (isRoundActive && (millis() - roundStartT) > availableTime)
  {
#ifdef DEBUG
    Serial.println("Timeout!");
#endif
    gameOver();
    return;
  }

  for (int b = 0; b < NUM_BUTTONS; b++)
  {
    if (isButtonPressed(b))
    {
      int pressedDigit = b + 1;
#ifdef DEBUG
      Serial.println("Pressed: " + String(pressedDigit));
#endif
      setGreenLed(pressedDigit, true);
      if (pressedDigit != currentSequence[currentSeqIndex])
      {
#ifdef DEBUG
        Serial.println("Wrong button!");
#endif
        gameOver();
        return;
      }
      else
      {
        currentSeqIndex++;
        if (currentSeqIndex >= 4)
        {
          score++;
          displaySuccess(score);
#ifdef DEBUG
          Serial.println("Correct! Score: " + String(score));
#endif
          availableTime *= levelfactor;
          if (availableTime < 500)
            availableTime = 500;
#ifdef DEBUG
          Serial.println("Available time: " + String(availableTime / 1000.0) + " s");
#endif
          isRoundActive = false;
          delay(INTER_ROUND_DELAY);
        }
      }
    }
  }
}

void gameOver()
{
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

void sleepNow()
{
  allLedsOff();
  lcdSleep();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
#ifdef DEBUG
  Serial.println("Going to sleep...");
#endif
  prepareSleep();
  sleep_mode();
#ifdef WOKWI_SIMULATION
  while (!isButtonPressed(0)) // busy wait to simulate sleep
    delay(200);
#endif
  sleep_disable();
  initButtons();
#ifdef DEBUG
  Serial.println("Woke up!");
#endif
  lcdWake();
  changeState(INTRO_STATE);
}
