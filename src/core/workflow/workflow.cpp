#include "workflow.h"

static Workflow* activeWorkflow = nullptr;
static uint8_t powerOnInitialBrightness, powerOffInitialBrightness;
static uint8_t prevSec;

void workflowTicker() {
    if (prevSec != now.sec) {
        prevSec = now.sec;
        if (now.sec == 0) {
            if (isTimeReady) {
                DEBUG(F("day: ")); DEBUG(now.day);
                DEBUG(F(", hour: ")); DEBUG(now.hour);
                DEBUG(F(", min: ")); DEBUG(now.min);
                DEBUG(F(", sec: ")); DEBUGLN(now.sec);
                checkWorkflowStart(); // check if it's time to start some workflow
            }
        }
    }
}

void checkWorkflowStart() {
    // if already performing then do nothing
    if (isWorkflowRunning()) return;
    
    auto currentWorkflow = workflowsService.isWorkflow(); // check

    // if has active workflow
    if (currentWorkflow != nullptr) {

        // is "dawn" && is turned off && "sunset" is not in progress
        if (cfg.brightness <= currentWorkflow->brightness && !workflowPowerOffTmr.isRunning()) {
            // check if no duration for action
            if (currentWorkflow->duration <= 0) {
                cfg.power = true;
                cfg.brightness = currentWorkflow->brightness;
                EE_updateCfg();
                return;
            }

            // if same brightness set -> consider as "dawn" so set brightness to 0
            if (cfg.brightness == currentWorkflow->brightness) {
                cfg.brightness = 0;
            }

            // set active workflow
            activeWorkflow = currentWorkflow;

            // start "dawn"
            float brightnessFactor = (float)(currentWorkflow->brightness - cfg.brightness) / 255;
            uint32_t estimatedTime = (float)(currentWorkflow->duration / brightnessFactor) * 60000ul;
            DEBUG(F("Dawn start! Estimated time: ")); DEBUGLN(estimatedTime);

            // set timers duration for "dawn"
            workflowPowerOnTmr.setPeriod(estimatedTime);

            // start timers
            workflowPowerOnTmr.restart();
            workflowTickerTmr.restart();

            // remember the start brightness
            powerOnInitialBrightness = cfg.brightness;

            // power on
            cfg.power = true;
        } 

        // is "sunset" && is turned on && "dawn" is not in progress
        else if (cfg.brightness > currentWorkflow->brightness && !workflowPowerOnTmr.isRunning()) {
            // check if no duration for action
            if (currentWorkflow->duration <= 0) {
                cfg.brightness = currentWorkflow->brightness;
                if (currentWorkflow->brightness <= 0) {
                    cfg.power = false;
                }
                EE_updateCfg();
                return;
            }

            // set active workflow
            activeWorkflow = currentWorkflow;

            // start "sunset"
            float brightnessFactor = (float)(cfg.brightness - currentWorkflow->brightness) / 255;
            uint32_t estimatedTime = (float)(currentWorkflow->duration / brightnessFactor) * 60000ul;
            DEBUG(F("Sunset start! Estimated time: ")); DEBUGLN(estimatedTime);

            // set timers duration for "sunset"
            workflowPowerOffTmr.setPeriod(estimatedTime);

            // start timers
            workflowPowerOffTmr.restart();
            workflowTickerTmr.restart();

            // remember the start brightness
            powerOffInitialBrightness = cfg.brightness;
        }

    }
}

void checkWorkflowTimers() {
    // if "dawn" or "sunset" is in progress
    if (workflowTickerTmr.period() && activeWorkflow != nullptr) {
        // "dawn" check
        if (workflowPowerOnTmr.isRunning()) {
            // map brightness
            auto timerBrightnessValue = powerOnInitialBrightness + workflowPowerOnTmr.getLength8();
            auto isFinishedForBrightness = timerBrightnessValue >= activeWorkflow->brightness;
            auto brightness = isFinishedForBrightness ? activeWorkflow->brightness : timerBrightnessValue;

            DEBUG(F("<Dawn> brightness: ")); DEBUGLN(brightness);

            // set brightness
            cfg.brightness = brightness;

            // if is finished
            if (isFinishedForBrightness || workflowPowerOnTmr.elapsed())
            {
                activeWorkflow = nullptr;
                workflowTickerTmr.stop();  // stop ticker
                workflowPowerOnTmr.stop(); // stop "dawn"
                EE_updateCfg();
            }
        }

        // Power off check
        else if (workflowPowerOffTmr.isRunning()) {
            // map brightness
            auto timerBrightnessValue = powerOffInitialBrightness - workflowPowerOffTmr.getLength8();
            auto isFinishedForBrightness = timerBrightnessValue <= activeWorkflow->brightness;
            auto brightness = isFinishedForBrightness ? activeWorkflow->brightness : timerBrightnessValue;

            DEBUG(F("<Sunset> brightness: ")); DEBUGLN(brightness);

            // set brightness
            cfg.brightness = brightness;

            // if is finished
            if (isFinishedForBrightness || workflowPowerOffTmr.elapsed()) {
                if (cfg.brightness == 0) {
                    cfg.power = false;
                }
                activeWorkflow = nullptr;
                workflowTickerTmr.stop();   // stop ticker
                workflowPowerOffTmr.stop(); // stop power on
                EE_updateCfg();
            }
        }
    }
}

void stopWorkflowProcess() {
    workflowTickerTmr.stop();
    workflowPowerOnTmr.stop();
    workflowPowerOffTmr.stop();
}

bool isWorkflowRunning() {
    return workflowPowerOnTmr.isRunning() || workflowPowerOffTmr.isRunning();
}