#include "helpers.h"

bool smoothBrightness(uint8_t brightnessToChange, uint8_t minBrightness) {
    if (led.getBrightness() != brightnessToChange) {
        int8_t dir = led.getBrightness() < brightnessToChange ? BRIGHTNESS_CHANGE_STEP : -BRIGHTNESS_CHANGE_STEP;
        uint8_t briValue = constrain(led.getBrightness() + dir, minBrightness, BRIGHTNESS_CHANGE_MAX);
        led.setBrightness(briValue);
        if (briValue == 0) led.clear();
        return true;
    }
    return false;
}

void factoryReset() {
    Config newCfg;

    // preserve wifi and AP
    strncpy(newCfg.ssid, cfg.ssid, sizeof(newCfg.ssid) - 1);
    strncpy(newCfg.pass, cfg.pass, sizeof(newCfg.pass) - 1);
    strncpy(newCfg.apName, cfg.apName, sizeof(newCfg.apName) - 1);
    strncpy(newCfg.apPass, cfg.apPass, sizeof(newCfg.apPass) - 1);
    newCfg.ssid[sizeof(newCfg.ssid) - 1] = '\0';
    newCfg.pass[sizeof(newCfg.pass) - 1] = '\0';
    newCfg.apName[sizeof(newCfg.apName) - 1] = '\0';
    newCfg.apPass[sizeof(newCfg.apPass) - 1] = '\0';

    // preserve MQTT
    strncpy(newCfg.host, cfg.host, sizeof(newCfg.host) - 1);
    strncpy(newCfg.local, cfg.local, sizeof(newCfg.local) - 1);
    strncpy(newCfg.remote, cfg.remote, sizeof(newCfg.remote) - 1);
    newCfg.host[sizeof(newCfg.host) - 1] = '\0';
    newCfg.local[sizeof(newCfg.local) - 1] = '\0';
    newCfg.remote[sizeof(newCfg.remote) - 1] = '\0';
    newCfg.port = cfg.port;

    // preserve button
    newCfg.useBtn = cfg.useBtn;
    newCfg.btnPin = cfg.btnPin;

    // preserve led
    newCfg.ledCount = cfg.ledCount;

    delay(50);
    modesLocalReset();
    for (uint16_t i = 0; i < eeSize; i++) {
        EEPROM.write(i, i == 0 ? EE_KEY : 0);
    }
    EEPROM.put(1, newCfg);
    EEPROM.put(sizeof(newCfg) + sizeof(workflows) + 1, modes);
    EEPROM.commit();
    delay(100);
    ESP.restart();
}

void modesLocalReset() {
    for (uint8_t i = 0; i < MODE_AMOUNT; ++i) {
        modes[i].Speed = defaultModes[i].Speed;
        modes[i].Scale = defaultModes[i].Scale;
    }
}