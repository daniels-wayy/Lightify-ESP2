#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "timer.h"
#include "led.h"
#include "config.h"
#include "data.h"
#include "effects.h"
#include "timer.h"

extern LedService led;
extern Config cfg;
extern LedEffects effects;
extern Timer workflowPowerOnTmr;
extern Timer workflowPowerOffTmr;

void ledTick();