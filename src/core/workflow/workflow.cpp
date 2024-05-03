#include "workflow.h"

static uint8_t _prevSec;

void checkWorkflow() {
    if (_prevSec != now.sec) {
        _prevSec = now.sec;
        if (!isWorkflowRunning() && now.sec == 0 && isTimeReady) {

            DEBUG(F("day: ")); DEBUG(now.day);
            DEBUG(F(", hour: ")); DEBUG(now.hour);
            DEBUG(F(", min: ")); DEBUG(now.min);
            DEBUG(F(", sec: ")); DEBUGLN(now.sec);
            
            auto scheduledWorkflow = workflowsService.isWorkflow(); // check

            // if should perform workflow
            if (scheduledWorkflow != nullptr) {
                currentWorkflow = scheduledWorkflow; // save
                bool isSameState = cfg.brightness == currentWorkflow->brightness;

                if (!isSameState) {
                    bool isDawn = cfg.brightness < currentWorkflow->brightness;
                    auto estimatedTime = _calculateEstimatedTime(currentWorkflow, isDawn); // calculate duration in ms

                    // if no duration for workflow
                    if (estimatedTime <= 0) {
                        DEBUGLN(F("Zero duration workflow start"));
                        cfg.power = isDawn ? true : currentWorkflow->brightness <= 0 ? false : cfg.power;
                        cfg.brightness = constrain(currentWorkflow->brightness, BRIGHTNESS_CHANGE_MIN, BRIGHTNESS_CHANGE_MAX);
                        currentWorkflow = nullptr;
                        EE_updateCfg();
                    } 
                    // dawn
                    else if (isDawn) {
                        cfg.power = true; // ensure device is turned on
                        DEBUG(F("Dawn start! Estimated time: ")); DEBUGLN(estimatedTime);
                        dawnInitialBrightness = cfg.brightness;
                        dawnTimer.setPeriod(estimatedTime);
                        dawnTimer.restart();
                        workflowTimer.restart();
                    } 
                    // sunset
                    else {
                        DEBUG(F("Sunset start! Estimated time: ")); DEBUGLN(estimatedTime);
                        sunsetInitialBrightness = cfg.brightness;
                        sunsetTimer.setPeriod(estimatedTime);
                        sunsetTimer.restart();
                        workflowTimer.restart();
                    }
                } else {
                    cfg.power = currentWorkflow->brightness > 0;
                    currentWorkflow = nullptr;
                    EE_updateCfg();
                }
            }
        }
    }
}

uint32_t _calculateEstimatedTime(Workflow* workflow, bool isDawn) {
    if (workflow->duration <= 0) {
        return 0;
    }
    float brightnessFactor = (float)(isDawn ? (workflow->brightness - cfg.brightness) : (cfg.brightness - workflow->brightness)) / 255;
    uint32_t estimatedTime = (float)(workflow->duration / brightnessFactor) * 60000ul;
    return estimatedTime;
}

void stopWorkflowProcess() {
    workflowTimer.stop();
    dawnTimer.stop();
    sunsetTimer.stop();
}

bool isWorkflowRunning() {
    return dawnTimer.isRunning() || sunsetTimer.isRunning();
}