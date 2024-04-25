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
    _workflows[_cfg->workflowsCount].id = id;
    _workflows[_cfg->workflowsCount].isEnabled = isEnabled;
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
    uint8_t day,
    uint8_t hour,
    uint8_t minute,
    uint8_t duration,
    uint8_t brightness
) {
    for (int i = 0; i < _cfg->workflowsCount; i++) {
        if (_workflows[i].id == id) {
            _workflows[i].id = id;
            _workflows[i].isEnabled = isEnabled;
            _workflows[i].day = day;
            _workflows[i].hour = hour;
            _workflows[i].minute = minute;
            _workflows[i].duration = duration;
            _workflows[i].brightness = brightness;
        }
    }
}

void WorkflowsService::del(uint8_t id) {
    for (uint8_t i = 0; i < _cfg->workflowsCount; i++) {
        if (_workflows[i].id == id) {
            for (uint8_t j = i; j < _cfg->workflowsCount - 1; j++) {
                _workflows[j] = _workflows[j + 1];
            }
            _cfg->workflowsCount--;
            break;
        }
    }
}

void WorkflowsService::clear() {
    for (int i = 0; i < MAX_WORKFLOWS; i++) {
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
        // workflow has id and is enabled
        if (_workflows[i].id > 0 && _workflows[i].isEnabled) {
            if (
                // if is everyday
                _workflows[i].day == WORKFLOW_EVERYDAY_NUM || // or
                // if the same day
                _nowTime.day == _workflows[i].day || // or
                // if every weekdays selected
                (_workflows[i].day == WORKFLOW_EVERY_WEEKDAYS_NUM && _nowTime.day > 0 && _nowTime.day < 6) || // or
                // if every weekends selected
                (_workflows[i].day == WORKFLOW_EVERY_WEEKENDS_NUM && (_nowTime.day == 6 || _nowTime.day == 0))) {

                    // calculate minute
                    int desiredWorkflowMinute = _workflows[i].hour * 60 + _workflows[i].minute - _workflows[i].duration;
                    if (desiredWorkflowMinute < 0)
                        desiredWorkflowMinute += 1440;

                    // if found exact minute    
                    if (desiredWorkflowMinute == (_nowTime.hour * 60 + _nowTime.min)) {
                        return &_workflows[i];
                    }
            }
        }
    }
    return nullptr;
}