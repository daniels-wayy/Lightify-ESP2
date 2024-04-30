#include "workflows.h"

WorkflowsService::WorkflowsService(Config &cfg, Workflow *workflows, Time &nowTime)
    : _cfg(&cfg), _workflows(workflows), _nowTime(nowTime) {}

void WorkflowsService::add(
    uint8_t id,
    uint8_t isEnabled,
    uint8_t day,
    uint8_t hour,
    uint8_t minute,
    uint8_t duration,
    uint8_t brightness
) {
    if (!isAbleToAdd()) return;
    _workflows[_cfg->workflowsCount].id = constrain(id, 0, 255);
    _workflows[_cfg->workflowsCount].isEnabled = constrain(isEnabled, 0, 1);
    _workflows[_cfg->workflowsCount].day = constrain(day, 0, WORKFLOW_EVERYDAY_NUM);
    _workflows[_cfg->workflowsCount].hour = constrain(hour, 0, 23);
    _workflows[_cfg->workflowsCount].minute = constrain(minute, 0, 59);
    _workflows[_cfg->workflowsCount].duration = constrain(duration, 0, WORKFLOW_MAX_DURATION);
    _workflows[_cfg->workflowsCount].brightness = constrain(brightness, 0, 255);
    _cfg->workflowsCount++;
}

void WorkflowsService::update(
    uint8_t id,
    uint8_t isEnabled,
    uint8_t day,
    uint8_t hour,
    uint8_t minute,
    uint8_t duration,
    uint8_t brightness
) {
    for (int i = 0; i < MAX_WORKFLOWS; i++) {
        if (_workflows[i].id == id) {
            _workflows[i].id = constrain(id, 0, 255);
            _workflows[i].isEnabled = constrain(isEnabled, 0, 1);
            _workflows[i].day = constrain(day, 0, WORKFLOW_EVERYDAY_NUM);
            _workflows[i].hour = constrain(hour, 0, 23);
            _workflows[i].minute = constrain(minute, 0, 59);
            _workflows[i].duration = constrain(duration, 0, WORKFLOW_MAX_DURATION);
            _workflows[i].brightness = constrain(brightness, 0, 255);
        }
    }
}

void WorkflowsService::del(uint8_t id) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MAX_WORKFLOWS; i++) {
        if (_workflows[i].id == id) {
            _workflows[i] = Workflow();
        }
        if (_workflows[i].id > 0) count++;
    }
    _cfg->workflowsCount = count;
}

void WorkflowsService::clear() {
    for (uint8_t i = 0; i < MAX_WORKFLOWS; i++) {
        _workflows[i] = Workflow();
    }
    _cfg->workflowsCount = 0;
}

Workflow *WorkflowsService::getWorkflows() {
    return _workflows;
}

bool WorkflowsService::isAbleToAdd() {
    return _cfg->workflowsCount < MAX_WORKFLOWS;
}

Workflow *WorkflowsService::isWorkflow() {
    for (uint8_t i = 0; i < MAX_WORKFLOWS; i++) {
        auto _workflow = &_workflows[i];
        // workflow has id and is enabled
        if (_workflow->isUnique() && _workflow->isEnabled) {
            if (_workflow->isMatchingDay(_nowTime.day)) {
                // calculate minute
                int desiredWorkflowMinute = _workflow->hour * 60 + _workflow->minute - _workflow->duration;
                if (desiredWorkflowMinute < 0)
                    desiredWorkflowMinute += 1440;

                uint16_t nowMinute = _nowTime.hour * 60 + _nowTime.min;
                if (desiredWorkflowMinute == nowMinute) { // found exact minute
                    return _workflow;
                }
            }
        }
    }
    return nullptr;
}