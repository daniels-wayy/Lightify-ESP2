#include "led_ticker.h"

#define UPD_PERIOD 10

void ledTick() {
    bool isFxRunning = cfg.fxIndex > 0;
    if (isFxRunning) _effectsTick();

    static uint32_t updTimer;
    if (millis() - updTimer >= UPD_PERIOD) {
        updTimer = millis();
        if (!isFxRunning) _colorTick();
        uint8_t thisBri = isFxRunning && !cfg.power ? 0 : cfg.brightness;
        smoothBrightness(thisBri, 0);
        led.update();
        yield();
    }
}

void _colorTick() {
    CRGB ncol = cfg.currentColor();
    CRGB ccol = led.getLeds()[0];
    if (ccol != ncol) {
        ccol = blend(ccol, ncol, 32);
        led.fill(ccol);
    }    
}

void _effectsTick() {
    if (!cfg.power) return;

    static uint32_t tmr;
    if (!(millis() - tmr >= modes[cfg.fxIndex].Speed)) return;
    tmr = millis();

    switch (cfg.fxIndex) {
        case SPARKLES_INDEX:
            effects.sparkles(modes[SPARKLES_INDEX].Scale);
            break;
        case RAINBOW_INDEX:
            effects.rainbow(modes[RAINBOW_INDEX].Scale);
            break;
        case FIRE_INDEX:
            effects.fire(modes[FIRE_INDEX].Scale);
            break;
        default:
            break;
    }
}