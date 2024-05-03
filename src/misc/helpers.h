#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "led.h"
#include "data.h"
#include "misc/utils.h"

static const ModeType defaultModes[] = {
  ModeType(),
  ModeType(),
  ModeType(),
  ModeType(),
};

extern ModeType modes[MODE_AMOUNT];
extern Config cfg;
extern LedService led;
extern Workflow workflows[MAX_WORKFLOWS];
extern const uint16_t eeSize;

bool smoothBrightness(uint8_t brightnessToChange = cfg.brightness, uint8_t minBrightness = BRIGHTNESS_CHANGE_MIN);
void factoryReset();
void modesLocalReset();