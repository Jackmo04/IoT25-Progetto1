#include "Arduino.h"
#include "include/display.h"
#include "include/config.h"

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);

unsigned long lastDisplayLevel = 0;

void initDisplay(){
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void displayWelcome(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to TOS!");
  lcd.setCursor(0, 1);
  lcd.print("Press B1 to Start");
}

void displayGo(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Go!");
}

void displaySequence(int seq[NUM_BUTTONS]){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sequence:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < NUM_BUTTONS; i++){
    lcd.print(seq[i]);
  }
}

void displaySuccess(int score){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GOOD!");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
}

void displayGameOver(int score){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
  lcd.print("Final Score: ");
  lcd.print(score);
}

void displayScore(int score){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(score);
}

void displayLevel(int level, unsigned long startingTime){
  if (millis() - startingTime >= 2000) {
    displayWelcome();
    return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(level);
}

void lcdSleep(){
  lcd.setBacklight(0);
}

void lcdWake(){
  lcd.setBacklight(1);
}
