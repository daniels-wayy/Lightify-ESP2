#include "led_ticker.h"

void ledTick() {
    if (cfg.fxIndex > 0) return;
 
    static Timer refreshTimer(15);

    if (refreshTimer.period()) {
        CRGB ncol = cfg.currentColor();
        CRGB ccol = led.getLeds()[0];
        if (ccol != ncol)
            ccol = blend(ccol, ncol, 32);
        led.fill(ccol);
        smoothBrightness();
        led.update();
    }
}