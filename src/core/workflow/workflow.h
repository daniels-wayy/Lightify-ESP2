#pragma once

#include <Arduino.h>
#include "time.h"
#include "timer.h"
#include "config.h"
#include "workflows.h"
#include "misc/utils.h"
#include "core/eeprom/eeprom_ticker.h"

extern Time now;
extern Config cfg;
extern bool isTimeReady;
extern WorkflowsService workflowsService;
extern uint8_t dawnInitialBrightness, sunsetInitialBrightness;
extern Timer workflowTimer, dawnTimer, sunsetTimer;
extern Workflow* currentWorkflow;

void checkWorkflow();
uint32_t _calculateEstimatedTime(Workflow *workflow, bool isDawn);
void stopWorkflowProcess();
bool isWorkflowRunning();