#pragma once

#include <Arduino.h>
#include "config.h"
#include "data.h"

class WorkflowsService
{
public:
    WorkflowsService(
        Config &cfg,
        Workflow *workflows);

    void add(
        uint8_t id,
        uint8_t isEnabled,
        uint8_t isPowerOn,
        uint8_t day,
        uint8_t hour,
        uint8_t minute,
        uint8_t duration,
        uint8_t brightness);

    void update(
        uint8_t id,
        uint8_t isEnabled,
        uint8_t isPowerOn,
        uint8_t day,
        uint8_t hour,
        uint8_t minute,
        uint8_t duration,
        uint8_t brightness); 

    void del(uint8_t id);    

    bool isAbleToAdd();    
    Workflow* getWorkflows();

private:
    Config *_cfg;
    Workflow *_workflows;
};