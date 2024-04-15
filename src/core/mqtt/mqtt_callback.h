#pragma once

#include <Arduino.h>
#include "data.h"
#include "mqtt.h"
#include "timer.h"
#include "misc/utils.h"
#include "core/eeprom/eeprom_ticker.h"
#include "core/workflow/workflow.h"

extern MQTTService mqtt;

void mqttCallback(char *topic, byte *payload, uint16_t len);
