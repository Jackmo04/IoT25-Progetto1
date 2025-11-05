#include "Arduino.h"
#include "include/pot.h"
#include "include/config.h"

int readLevelFromPot(){
  int v = analogRead(POT_PIN);
  int level = map(v, 0, 1023, 1, 4);
  return level;
}
