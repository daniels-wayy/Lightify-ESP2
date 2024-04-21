#pragma once

#include <Arduino.h>
#include "misc/utils.h"
#include "led.h"

extern LedService led;

void onFirmwareUpdateStarted();
void onFirmwareUpdateProgress(uint8_t percent);
void onFirmwareUpdateFinished(bool isSuccess);