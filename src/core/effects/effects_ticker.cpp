#include "effects_ticker.h"

#define UPD_PERIOD 40

void effectsTicker() {
    // if no effect selected or no power -> do not show
    if (cfg.fxIndex <= 0 || !cfg.power) {
        if (!cfg.power && cfg.fxIndex > 0) {
            effects.setSmoothBrightness(led.getBrightness(), 0);
            led.clear();
            led.update();
        }
        return;
    }

    static Timer refreshTimer(UPD_PERIOD);

    // effects that are requires timer period to be speed
    if (cfg.fxIndex == SPARKLES_INDEX || 
        cfg.fxIndex == RAINBOW_INDEX || 
        cfg.fxIndex == FIRE_ONE_INDEX
    )
    {
        uint8_t spd = EFFECTS_MAX_SPEED - modes[cfg.fxIndex].Speed;
        refreshTimer.setPeriod(spd <= EFFECTS_MIN_SPEED ? EFFECTS_MIN_SPEED : spd);
    }
    else
    {
        refreshTimer.setPeriod(UPD_PERIOD);
    }

    if (refreshTimer.period())
    {
        switch (cfg.fxIndex)
        {
        case LAVA_INDEX:
            effects.lavaNoise(modes[LAVA_INDEX].Scale, modes[LAVA_INDEX].Speed);
            break;
        case CLOUDS_INDEX:
            effects.cloudsNoise(modes[CLOUDS_INDEX].Scale, modes[CLOUDS_INDEX].Speed);
            break;
        case FOREST_INDEX:
            effects.forestNoise(modes[FOREST_INDEX].Scale, modes[FOREST_INDEX].Speed);
            break;
        case OCEAN_INDEX:
            effects.oceanNoise(modes[OCEAN_INDEX].Scale, modes[OCEAN_INDEX].Speed);
            break;
        case SPARKLES_INDEX:
            effects.sparkles(modes[SPARKLES_INDEX].Scale);
            break;
        case RAINBOW_INDEX:
            effects.rainbow(modes[RAINBOW_INDEX].Scale);
            break;
        case FIRE_ONE_INDEX:
            effects.fire(modes[FIRE_ONE_INDEX].Scale);
            break;
        default:
            break;
        }
        
        effects.setSmoothBrightness(led.getBrightness(), cfg.brightness);
        led.update();
    }
}