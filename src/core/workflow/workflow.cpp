#include "workflow.h"

void workflowTicker()
{
    if (prevSec != now.sec)
    {
        prevSec = now.sec;
        if (now.sec == 0)
        {
            if (isTimeReady)
            {
                DEBUG("day: ");
                DEBUG(now.day);
                DEBUG(", hour: ");
                DEBUG(now.hour);
                DEBUG(", min: ");
                DEBUG(now.min);
                DEBUG(", sec: ");
                DEBUGLN(now.sec);

                checkWorkflowStart(); // check if it's time to start some workflow
            }
        }
    }
}

void checkWorkflowStart() {
    // if already performing then do nothing
    if (workflowPowerOnTmr.isRunning() || workflowPowerOffTmr.isRunning()) {
        return;
    }
    
    auto currentWorkflow = isWorkflow(); // check

    // if has active workflow
    if (currentWorkflow != nullptr) {

        // is "dawn" && is turned off && "sunset" is not in progress
        if (cfg.brightness < currentWorkflow->brightness /*currentWorkflow->isPowerOn*/ /*&& !cfg.power*/ && !workflowPowerOffTmr.isRunning()) {
            // check if no duration for action
            if (currentWorkflow->duration <= 0) {
                cfg.power = true;
                cfg.brightness = currentWorkflow->brightness;
                EE_updateCfg();
                return;
            }

            // set active workflow
            activeWorkflow = currentWorkflow;

            // start "dawn"
            float brightnessFactor = (float)(currentWorkflow->brightness - cfg.brightness) / 255;
            uint32_t estimatedTime = (float)(currentWorkflow->duration / brightnessFactor) * 60000ul;
            DEBUG("Dawn start! EstimatedTime: ");
            DEBUGLN(estimatedTime);

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
        if (cfg.brightness > currentWorkflow->brightness /*!currentWorkflow->isPowerOn*/ && cfg.power && !workflowPowerOnTmr.isRunning()) {
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
            DEBUG("Sunset start! EstimatedTime: ");
            DEBUGLN(estimatedTime);

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

void checkWorkflowTimers()
{
    // if "dawn" or "sunset" is in progress
    if (workflowPowerOnTmr.isRunning() || workflowPowerOffTmr.isRunning())
    {
        // if has active workflow and is ticked
        if (activeWorkflow != nullptr && workflowTickerTmr.period())
        {
            // "dawn" check
            if (workflowPowerOnTmr.isRunning())
            {
                // map brightness
                auto timerBrightnessValue = powerOnInitialBrightness + workflowPowerOnTmr.getLength8();
                auto isFinishedForBrightness = timerBrightnessValue >= activeWorkflow->brightness;
                auto brightness = isFinishedForBrightness ? activeWorkflow->brightness : timerBrightnessValue;

                DEBUG("<Dawn> brightness: ");
                DEBUGLN(brightness);

                // set brightness
                cfg.brightness = brightness;

                // if is finished
                if (isFinishedForBrightness || workflowPowerOnTmr.elapsed())
                {
                    workflowTickerTmr.stop();  // stop ticker
                    workflowPowerOnTmr.stop(); // stop "dawn"
                    EE_updateCfg();
                }
            }

            // Power off check
            else if (workflowPowerOffTmr.isRunning())
            {
                // map brightness
                auto timerBrightnessValue = powerOffInitialBrightness - workflowPowerOffTmr.getLength8();
                auto isFinishedForBrightness = timerBrightnessValue <= activeWorkflow->brightness;
                auto brightness = isFinishedForBrightness ? activeWorkflow->brightness : timerBrightnessValue;

                DEBUG("<Sunset> brightness: ");
                DEBUGLN(brightness);

                // set brightness
                cfg.brightness = brightness;

                // if is finished
                if (isFinishedForBrightness || workflowPowerOffTmr.elapsed())
                {
                    if (cfg.brightness == 0) {
                        cfg.power = false;
                    }
                    workflowTickerTmr.stop();   // stop ticker
                    workflowPowerOffTmr.stop(); // stop power on
                    EE_updateCfg();
                }
            }
        }
    }
}

void stopWorkflowProcess()
{
    if (workflowPowerOnTmr.isRunning() || workflowPowerOffTmr.isRunning())
    {
        workflowTickerTmr.stop();
        workflowPowerOnTmr.stop();
        workflowPowerOffTmr.stop();
    }
}

// checks if it's time to perform workflow item
Workflow *isWorkflow()
{
    for (uint8_t i = 0; i < MAX_WORKFLOWS; i++)
    {
        // workflow has id and is enabled
        if (workflows[i].id > 0 && workflows[i].isEnabled)
        {
            if (
                // if is everyday
                workflows[i].day == WORKFLOW_EVERYDAY_NUM || // or
                // if the same day
                now.day == workflows[i].day || // or
                // if every weekdays selected
                (workflows[i].day == WORKFLOW_EVERY_WEEKDAYS_NUM && now.day > 0 && now.day < 6) || // or
                // if every weekends selected
                (workflows[i].day == WORKFLOW_EVERY_WEEKENDS_NUM && (now.day == 6 || now.day == 0)))
            {

                // calculate minute
                int desiredWorkflowMinute = workflows[i].hour * 60 + workflows[i].minute - workflows[i].duration;

                if (desiredWorkflowMinute < 0)
                    desiredWorkflowMinute += 1440;

                // if found exact minute    
                if (desiredWorkflowMinute == now.hour * 60 + now.min)
                {
                    return &workflows[i];
                }
            }
        }
    }

    return nullptr;
}