#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "led.h"
#include "timer.h"
#include "data.h"
#include "config.h"

#define SPARKLES_INDEX 1
#define RAINBOW_INDEX 2
#define FIRE_INDEX 3

// Sparkles fx
#define SPARKLES_COLOR CRGB::White

// Fire fx
#define MIN_SAT 245
#define MAX_SAT 255
#define MIN_BRIGHT 70
#define MAX_BRIGHT 255
#define FIRE_STEP 15
#define HUE_START 0

class LedEffects {
public:
    LedEffects(LedService &strip, Config &cfg);
    void loadingEffect(CRGB color);
    void sparkles(uint8_t scale);
    void rainbow(uint8_t scale);
    void fire(uint8_t scale);
    void loopSmoothBrightness(uint8_t oldBri, uint8_t newBri);

private:
    LedService *_strip;
    Config* _cfg;
    CHSV _getFireColor(int val);
};