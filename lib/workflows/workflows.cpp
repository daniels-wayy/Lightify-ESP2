#include "workflows.h"

WorkflowsService::WorkflowsService(
    Config &cfg,
    Workflow *workflows)
    : _cfg(&cfg),
      _workflows(workflows)
{
}

void WorkflowsService::add(
    uint8_t id,
    uint8_t isEnabled,
    uint8_t isPowerOn,
    uint8_t day,
    uint8_t hour,
    uint8_t minute,
    uint8_t duration,
    uint8_t brightness)
{
    _workflows[_cfg->workflowsCount].id = id;
    _workflows[_cfg->workflowsCount].isEnabled = isEnabled;
    _workflows[_cfg->workflowsCount].isPowerOn = isPowerOn;
    _workflows[_cfg->workflowsCount].day = day;
    _workflows[_cfg->workflowsCount].hour = hour;
    _workflows[_cfg->workflowsCount].minute = minute;
    _workflows[_cfg->workflowsCount].duration = duration;
    _workflows[_cfg->workflowsCount].brightness = brightness;
    _cfg->workflowsCount++;
}

void WorkflowsService::update(
    uint8_t id,
    uint8_t isEnabled,
    uint8_t isPowerOn,
    uint8_t day,
    uint8_t hour,
    uint8_t minute,
    uint8_t duration,
    uint8_t brightness)
{
    for (int i = 0; i < _cfg->workflowsCount; i++)
    {
        if (_workflows[i].id == id)
        {
            _workflows[i].id = id;
            _workflows[i].isEnabled = isEnabled;
            _workflows[i].isPowerOn = isPowerOn;
            _workflows[i].day = day;
            _workflows[i].hour = hour;
            _workflows[i].minute = minute;
            _workflows[i].duration = duration;
            _workflows[i].brightness = brightness;
        }
    }
}

void WorkflowsService::del(uint8_t id)
{
    int j = 0;
    for (int i = 0; i < _cfg->workflowsCount; i++)
    {
        if (_workflows[i].id != id)
        {
            _workflows[j++] = _workflows[i];
        }
    }
    _cfg->workflowsCount = j;
}

Workflow *WorkflowsService::getWorkflows()
{
    return _workflows;
}

bool WorkflowsService::isAbleToAdd()
{
    return _cfg->workflowsCount < MAX_WORKFLOWS;
}