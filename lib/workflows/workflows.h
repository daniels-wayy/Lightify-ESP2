#pragma once

#include <Arduino.h>
#include "config.h"
#include "data.h"
#include "time.h"

class WorkflowsService
{
public:
    WorkflowsService(
        Config &cfg,
        Workflow *workflows,
        Time &nowTime);

    void add(
        uint8_t id,
        uint8_t isEnabled,
        uint8_t day,
        uint8_t hour,
        uint8_t minute,
        uint8_t duration,
        uint8_t brightness);

    void update(
        uint8_t id,
        uint8_t isEnabled,
        uint8_t day,
        uint8_t hour,
        uint8_t minute,
        uint8_t duration,
        uint8_t brightness); 

    void del(uint8_t id);    

    bool isAbleToAdd();    
    Workflow* getWorkflows();
    Workflow* isWorkflow();

private:
    Config *_cfg;
    Workflow *_workflows;
    Time &_nowTime;
};