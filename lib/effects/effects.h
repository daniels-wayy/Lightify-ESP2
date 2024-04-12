#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "led.h"
#include "timer.h"
#include "data.h"
#include "config.h"

#define LAVA_INDEX 1
#define CLOUDS_INDEX 2
#define FOREST_INDEX 3
#define OCEAN_INDEX 4
#define SPARKLES_INDEX 5
#define RAINBOW_INDEX 6
#define FIRE_ONE_INDEX 7

// Sparkles fx
#define SPARKLES_COLOR CRGB::LightGoldenrodYellow

// Fire fx
#define MIN_SAT 245
#define MAX_SAT 255
#define MIN_BRIGHT 70
#define MAX_BRIGHT 255
#define FIRE_STEP 15
#define HUE_START 0

class LedEffects
{
public:
    LedEffects(LedService &strip, Config &cfg, bool &fxPropertyChanged);
    void loadingEffect(CRGB color);
    void lavaNoise(uint8_t scale, uint8_t speed);
    void cloudsNoise(uint8_t scale, uint8_t speed);
    void forestNoise(uint8_t scale, uint8_t speed);
    void oceanNoise(uint8_t scale, uint8_t speed);
    void sparkles(uint8_t scale);
    void rainbow(uint8_t scale);
    void fire(uint8_t scale);
    void setSmoothBrightness(uint8_t oldBri, uint8_t newBri);

private:
    LedService *_strip;
    Config* _cfg;
    bool& _fxPropertyChanged;

    void _fillNoiseLED();
    CHSV _getFireColor(int val);
};