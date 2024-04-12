#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "data.h"
#include "config.h"
#include "device_type.h"

class LedService
{
public:
    LedService(Config &cfg);
    ~LedService() {
        delete[] _leds;
    }
    void initialize();
    void setBrightness(uint8_t value);
    uint8_t getBrightness();
    void update();
    void clear();
    CRGB* getLeds();

    void fill(CRGB color);
    void fillPixel(uint16_t pix, CRGB color);

    void fillRainbow(uint8_t initialhue, uint8_t deltahue);

    uint16_t ledsCount() {
        return _cfg->ledCount;
    }

private:
    CRGB* _leds;
    Config* _cfg;
};