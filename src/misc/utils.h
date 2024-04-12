#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include "data.h"
#include "config.h"

#if USE_SERIAL == 1
#define DEBUG(x) Serial.print(x)
#define DEBUGLN(x) Serial.println(x)
#else
#define DEBUG(x)
#define DEBUGLN(x)
#endif
