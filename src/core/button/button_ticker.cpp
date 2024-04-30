#include "button_ticker.h"

void buttonTicker(Button *btn) {
    if (btn == nullptr) return;

    static bool holdFlag = 0, brDir = 0;
    static Timer stepTmr(60);

    btn->tick();

    if (btn->isClick()) {
        _onSingleClick();
    }
    if (cfg.power && btn->isHold()) {
        if (stepTmr.period()) {
            holdFlag = true;
            int temp = cfg.brightness;
            temp += brDir ? BRIGHTNESS_CHANGE_STEP : -BRIGHTNESS_CHANGE_STEP;
            temp = constrain(temp, BRIGHTNESS_CHANGE_MIN, BRIGHTNESS_CHANGE_MAX);
            cfg.brightness = temp;
        }
    } else {
        if (holdFlag) {
            holdFlag = false;
            brDir = !brDir;
            DEBUG("Brightness: ");
            DEBUGLN(cfg.brightness);
            EE_updateCfg();
        }
    }
}

void _onSingleClick() {
    cfg.power = !cfg.power;
    if (cfg.brightness == 0) cfg.brightness = BRIGHTNESS_CHANGE_MIN;
    if (isWorkflowRunning()) stopWorkflowProcess();
}