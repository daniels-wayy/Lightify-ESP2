#pragma once

#include <GyverPortal.h>
#include <mString.h>
#include "config.h"
#include "effects.h"
#include "mqtt.h"
#include "data.h"
#include "led.h"
#include "device_type.h"
#include "misc/utils.h"
#include "core/eeprom/eeprom_ticker.h"
#include "core/workflow/workflow.h"

extern Config cfg;
extern GyverPortal* portal;
extern LedEffects effects;
extern MQTTService mqtt;
extern LedService led;

void buildWebInterface();
void startLocalPortal(IPAddress ip);
void checkPortal();