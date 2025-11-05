#include "display.h"
#include <LiquidCrystal_I2C.h>
#include "config.h"

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);

void initDisplay(){
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
}

void showWelcome(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to TOS!");
  lcd.setCursor(0, 1);
  lcd.print("Press B1 to Start");
}

void showGo(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Go!");
}

void showSequence(int seq[NUM_BUTTONS]){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sequence:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < NUM_BUTTONS; i++){
    lcd.print(seq[i]);
  }
}

void showGood(int score){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GOOD!");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
}

void showGameOver(int score){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
  lcd.print("Final Score: ");
  lcd.print(score);
}

void showScore(int score){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(score);
}

void clearDisplay(){
  lcd.clear();
}
