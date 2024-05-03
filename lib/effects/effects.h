#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "led.h"
#include "timer.h"
#include "data.h"
#include "config.h"

#define SPARKLES_INDEX 1U
#define RAINBOW_INDEX 2U
#define FIRE_INDEX 3U

// Sparkles fx
#define SPARKLES_COLOR CRGB::White

// Fire fx
#define MIN_SAT 245U
#define MAX_SAT 255U
#define MIN_BRIGHT 70U
#define MAX_BRIGHT 255U
#define FIRE_STEP 15U
#define HUE_START 0U

class LedEffects {
public:
    LedEffects(LedService &strip, Config &cfg);
    void loadingEffect(CRGB color);
    void sparkles(uint8_t scale);
    void rainbow(uint8_t scale);
    void fire(uint8_t scale);

private:
    LedService *_strip;
    Config* _cfg;
    CHSV _getFireColor(int val);
};