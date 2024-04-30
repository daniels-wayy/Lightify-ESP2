#pragma once

#include <Arduino.h>
#include "button.h"
#include "config.h"
#include "timer.h"
#include "core/workflow/workflow.h"
#include "core/eeprom/eeprom_ticker.h"
#include "misc/utils.h"

extern Config cfg;

void buttonTicker(Button *btn);
void _onSingleClick();