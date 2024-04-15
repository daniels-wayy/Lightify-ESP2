#include "button_ticker.h"

void buttonTicker() {
    if (!cfg.useBtn) {
        return;
    }

    static bool holdFlag = 0, brDir = 0;
    static Timer stepTmr(80);

    btn.tick();

    if (btn.isClick()) {
        _onSingleClick();
    }
    if (cfg.power && btn.isHold()) {
        if (stepTmr.period()) {
            holdFlag = true;
            int temp = cfg.brightness;
            temp += brDir ? 5 : -5;
            temp = constrain(temp, 10, 255);
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

    if (cfg.brightness == 0) {
        cfg.brightness = 10;
    }

    if (isWorkflowRunning()) {
        stopWorkflowProcess();
    }
}