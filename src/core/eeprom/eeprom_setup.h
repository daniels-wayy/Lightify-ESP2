#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include "data.h"
#include "config.h"
#include "misc/utils.h"

extern bool EECfgFlag;
extern Config cfg;
extern Workflow workflows[MAX_WORKFLOWS];
extern ModeType modes[MODE_AMOUNT];

void EE_setup();
void EE_erase();