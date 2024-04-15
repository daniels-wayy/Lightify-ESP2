#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include "timer.h"
#include "led.h"

class OTAUpdater
{
public:
    OTAUpdater(LedService &strip);
    void update(
        char* url,
        std::function<void()> onStart,
        std::function<void(uint8_t)> onProgress,
        std::function<void()> onFinished,
        std::function<void(const char*)> onError
    );
private:
    LedService *_strip;
};