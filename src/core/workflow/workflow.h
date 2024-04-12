#pragma once

#include <Arduino.h>
#include "time.h"
#include "timer.h"
#include "config.h"
#include "led.h"
#include "effects.h"
#include "misc/utils.h"
#include "core/eeprom/eeprom_ticker.h"

extern Time now;
extern uint8_t prevSec;
extern bool isTimeReady;
extern Timer workflowPowerOnTmr;
extern Timer workflowPowerOffTmr;
extern Timer workflowTickerTmr;
extern Config cfg;
extern LedService led;
extern LedEffects effects;
extern uint8_t powerOffInitialBrightness;
extern uint8_t powerOnInitialBrightness;
extern Workflow workflows[MAX_WORKFLOWS];
extern Workflow* activeWorkflow;

void workflowTicker();
void checkWorkflowTimers();
void stopWorkflowProcess();
void checkWorkflowStart();

Workflow* isWorkflow();