#ifndef __CONFIG__
#define __CONFIG__

// #define __DEBUG__

#define NUM_BUTTONS 4

#define BUT01_PIN 2
#define BUT02_PIN 3
#define BUT03_PIN 4
#define BUT04_PIN 5

#define LED1_PIN 8
#define LED2_PIN 9
#define LED3_PIN 10
#define LED4_PIN 11

#define RED_LED_PIN 13

#define POT_PIN A0

// display LCD
#define LCD_I2C_ADDRESS 0x27 
#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_SDA A4
#define LCD_SCL A5

#endif
