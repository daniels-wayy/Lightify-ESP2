#include "effects.h"

LedEffects::LedEffects(LedService &strip, Config &cfg) : _strip(&strip), _cfg(&cfg) {}

void LedEffects::loadingEffect(CRGB color) {
    static int8_t dir = 1;
    static uint8_t val = MIN_LOAD_ANIM_BRI;
    static Timer tmr(20);
    if (tmr.period())
    {
        val += dir;
        if (val >= MAX_LOAD_ANIM_BRI || val <= MIN_LOAD_ANIM_BRI)
            dir = -dir;
        _strip->fill(color);
        _strip->setBrightness(val);
        _strip->update();
    }
    yield();
}

void LedEffects::sparkles(uint8_t fxScale) {
    if (random(0, 255) < (fxScale / 4))
    {
        uint16_t pos = random(_cfg->ledCount);
        _strip->getLeds()[pos] = blend(_cfg->currentColor(), SPARKLES_COLOR, 48);
    }
    for (uint16_t i = 0; i < _cfg->ledCount; i++)
    {
        if (_strip->getLeds()[i] != SPARKLES_COLOR)
        {
            _strip->getLeds()[i] = blend(_strip->getLeds()[i], _cfg->currentColor(), 48);
        }
    }
}

static uint8_t hue = 0;
void LedEffects::rainbow(uint8_t fxScale) {
    hue += 2;
    for (byte j = 0; j < _cfg->ledCount; j++) {
        auto thisColor = CHSV((byte)(hue + j * fxScale), 255, 255);
        _strip->fillPixel(j, thisColor);
    }
}

static int counter = 0;
void LedEffects::fire(uint8_t scale) {
    for (uint16_t i = 0; i < _cfg->ledCount; i++) {
        _strip->fillPixel(i, _getFireColor((inoise8(i * (uint8_t)(scale / 2), counter))));
    }
    counter += 20;
}

CHSV LedEffects::_getFireColor(int val) {
    return CHSV(
        HUE_START + map(val, 0, 255, 0, _cfg->colorHue),                // H
        constrain(map(val, 0, 255, MAX_SAT, MIN_SAT), 0, 255),          // S
        constrain(map(val, 0, 255, MIN_BRIGHT, MAX_BRIGHT), 0, 255)     // V
    );
}