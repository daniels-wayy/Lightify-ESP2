#include "workflow_ticker.h"

void workflowTick() {
    if (!workflowTimer.period() || currentWorkflow == nullptr) return;

    // is dawn
    if (dawnTimer.isRunning()) {
        uint8_t timerBrightnessValue = dawnInitialBrightness + dawnTimer.getLength8();
        bool isFinishedForBrightness = timerBrightnessValue >= currentWorkflow->brightness;
        uint8_t brightness = isFinishedForBrightness ? currentWorkflow->brightness : timerBrightnessValue;
        DEBUG(F("Dawn brightness: ")); DEBUGLN(brightness);

        cfg.brightness = brightness;

        if (isFinishedForBrightness || dawnTimer.elapsed()) {
            currentWorkflow = nullptr;
            workflowTimer.stop();
            dawnTimer.stop();
            EE_updateCfg();
        }
    } 
    // is sunset
    else if (sunsetTimer.isRunning()) {
        uint8_t timerBrightnessValue = sunsetInitialBrightness - sunsetTimer.getLength8();
        bool isFinishedForBrightness = timerBrightnessValue <= currentWorkflow->brightness;
        uint8_t brightness = isFinishedForBrightness ? currentWorkflow->brightness : timerBrightnessValue;
        DEBUG(F("Sunset brightness: ")); DEBUGLN(brightness);

        cfg.brightness = brightness;

        if (isFinishedForBrightness || sunsetTimer.elapsed()) {
            if (cfg.brightness == 0) cfg.power = false;
            currentWorkflow = nullptr;
            workflowTimer.stop();
            sunsetTimer.stop();
            EE_updateCfg();
        }
    }
}