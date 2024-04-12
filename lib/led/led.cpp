#include "led.h"

LedService::LedService(Config &cfg) : _cfg(&cfg)
{
    _leds = new CRGB[cfg.ledCount];
}

void LedService::initialize()
{
    FastLED.addLeds<WS2812, DEVICE_CONFIG.ledPin, GRB>(_leds, _cfg->ledCount).setCorrection(TypicalLEDStrip);
    if (_cfg->stripCurrent > 0) {
        FastLED.setMaxPowerInVoltsAndMilliamps(STRIP_VOLT, _cfg->stripCurrent);
    }
    setBrightness(50);
    clear();
    update();
}

void LedService::setBrightness(uint8_t value) {
    FastLED.setBrightness(value);
}

uint8_t LedService::getBrightness() {
    return FastLED.getBrightness();
}

void LedService::update() {
    FastLED.show();
}

void LedService::clear() {
    FastLED.clear();
}

CRGB* LedService::getLeds() {
    return _leds;
}

void LedService::fill(CRGB color) {
    fill_solid(_leds, _cfg->ledCount, color);
}

void LedService::fillPixel(uint16_t pix, CRGB color) {
    _leds[pix] = color;
}

void LedService::fillRainbow(uint8_t initialhue, uint8_t deltahue) {
    fill_rainbow(_leds, _cfg->ledCount, initialhue, deltahue);
}