#include "led_ticker.h"

#define CLR_UPD_PERIOD 20U
#define BRI_UPD_PERIOD 2U

void ledTick() {
    static uint32_t updTimer, briTimer;

    // brightness and power timer
    if (millis() - briTimer >= BRI_UPD_PERIOD) {
        briTimer = millis();
        if (smoothBrightness(cfg.power ? cfg.brightness : 0, 0)) {
           led.update();
           yield();
        }
    }

    // color and effects timer 
    if (millis() - updTimer >= (cfg.fxIndex > 0 ? modes[cfg.fxIndex].Speed : CLR_UPD_PERIOD)) {
        updTimer = millis();
        if (cfg.fxIndex > 0 ? _effectsTick() : _colorTick()) {
            led.update();
            yield();
        }
    }
}

bool _colorTick() {
    CRGB ncol = cfg.currentColor();
    CRGB ccol = led.getLeds()[0];
    if (ccol != ncol) {
        ccol = blend(ccol, ncol, 17);
        led.fill(ccol);
        return true;
    }
    return false;
}

bool _effectsTick() {
    if (!cfg.power) return false;

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

    return true;
}