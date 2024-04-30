#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include "timer.h"
#include "led.h"

class OTAUpdater {
public:
    OTAUpdater(LedService& ledService);
    void update(
        char* url,
        std::function<void()> onStart,
        std::function<void(uint8_t)> onProgress,
        std::function<void()> onFinished,
        std::function<void(const char*)> onError
    );
    void setOnFirmwareUpdateStarted(void (*callback)()) { _onFirmwareUpdateStarted = callback; }
    void setOnFirmwareUpdateProgress(void (*callback)(uint8_t)) { _onFirmwareUpdateProgress = callback; }
    void setOnFirmwareUpdateFinished(void (*callback)(bool)) { _onFirmwareUpdateFinished = callback; }
private:
    LedService *_ledService;
    bool _isUpdating = false;
    void (*_onFirmwareUpdateStarted)() = nullptr;
    void (*_onFirmwareUpdateProgress)(uint8_t) = nullptr;
    void (*_onFirmwareUpdateFinished)(bool) = nullptr;
};