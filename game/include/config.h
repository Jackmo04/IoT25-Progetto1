#ifndef __CONFIG__
#define __CONFIG__

// General settings
#define NUM_BUTTONS 4

// Pin assignments: buttons
#define BTN_01_PIN 2
#define BTN_02_PIN 3
#define BTN_03_PIN 4
#define BTN_04_PIN 5

// Pin assignments: LEDs
#define LED_1_PIN 8
#define LED_2_PIN 9
#define LED_3_PIN 10
#define LED_4_PIN 11
#define RED_LED_PIN 6

// Pin assignments: other components
#define POT_PIN A0

// LCD settings
#define LCD_I2C_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// Debug and simulation settings, comment/uncomment as needed
// #define DEBUG               // Enable debug serial output
// #define DEBUG_VERBOSE       // Enable verbose debug output
// #define WOKWI_SIMULATION    // Enable ONLY if running in Wokwi simulation environment

#endif
