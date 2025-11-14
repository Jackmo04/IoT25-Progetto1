#include "include/pot.h"
#include "include/config.h"

#include <Arduino.h>

int readLevelFromPot()
{
  int v = analogRead(POT_PIN);
  int level = map(v, 0, 1023, 1, 5);
  return level > 4 ? 4
    : level < 1 ? 1 
    : level;
}
