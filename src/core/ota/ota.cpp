#include "ota.h"

void onFirmwareUpdateStarted() {
    DEBUGLN(F("Firmware update started!"));
    led.clear();
    led.fill(CRGB::WhiteSmoke);
    led.setBrightness(0);
    led.update();
}

void onFirmwareUpdateProgress(uint8_t percent) {
    DEBUG(F("Firmware update progress: ")); DEBUG(percent); DEBUGLN(F("%"));
    led.setBrightness(uint8_t(percent * 2));
    led.update();
}

void onFirmwareUpdateFinished(bool isSuccess) {
    DEBUG(F("Firmware update finished! isSuccess: ")); DEBUGLN(isSuccess);
    led.fill(isSuccess ? CRGB::LightGreen : CRGB::DarkRed);
    led.update();
}