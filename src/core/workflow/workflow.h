#pragma once

#include <Arduino.h>
#include "time.h"
#include "timer.h"
#include "config.h"
#include "workflows.h"
#include "misc/utils.h"
#include "core/eeprom/eeprom_ticker.h"

extern Time now;
extern bool isTimeReady;
extern Timer workflowPowerOnTmr;
extern Timer workflowPowerOffTmr;
extern Timer workflowTickerTmr;
extern Config cfg;
extern Workflow workflows[MAX_WORKFLOWS];
extern WorkflowsService workflowsService;

void workflowTicker();
void checkWorkflowTimers();
void stopWorkflowProcess();
void checkWorkflowStart();
bool isWorkflowRunning();