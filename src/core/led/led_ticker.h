#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "timer.h"
#include "led.h"
#include "config.h"
#include "data.h"
#include "effects.h"
#include "misc/helpers.h"

extern LedService led;
extern Config cfg;
extern LedEffects effects;

void ledTick();
bool _colorTick();
bool _effectsTick();