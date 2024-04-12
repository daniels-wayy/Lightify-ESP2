#pragma once

#include <Arduino.h>
#include "timer.h"
#include "config.h"
#include "data.h"
#include "misc/utils.h"

extern Config cfg;
extern bool EECfgFlag;
extern bool EEWorkflowsFlag;
extern bool EEFxFlag;
extern Timer EETimer;
extern Workflow workflows[MAX_WORKFLOWS];
extern ModeType modes[MODE_AMOUNT];

// loop
void EE_ticker();

// cfg
void EE_updateCfg();
void EE_updateCfgNow();
void EE_updateCfgRst();

// workflows
void EE_updateWorkflows();
void EE_updateWorkflowsNow();

// effects
void EE_updateEffects();
void EE_updateEffectsNow();