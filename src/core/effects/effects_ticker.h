#pragma once

#include "effects.h"
#include "config.h"
#include "timer.h"
#include "led.h"

extern LedService led;
extern LedEffects effects;
extern Config cfg;
extern ModeType modes[MODE_AMOUNT];

void effectsTicker();