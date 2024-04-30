#pragma once

#include <Arduino.h>
#include "config.h"
#include "timer.h"
#include "misc/utils.h"
#include "core/eeprom/eeprom_ticker.h"

extern Config cfg;
extern Workflow* currentWorkflow;
extern Timer workflowTimer, dawnTimer, sunsetTimer;
extern uint8_t dawnInitialBrightness, sunsetInitialBrightness;

void workflowTick();