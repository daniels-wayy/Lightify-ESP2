#include "effects_ticker.h"

void effectsTicker() {
    if (cfg.fxIndex <= 0 || led.isOff()) return;

    if (!cfg.power) {
        effects.setSmoothBrightness(led.getBrightness(), 0);
        led.clear();
        led.update();
        return;
    }

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

    effects.setSmoothBrightness(led.getBrightness(), cfg.brightness);
    led.update();
}