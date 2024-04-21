#pragma once

#include <Arduino.h>
#include "mqtt.h"
#include "misc/utils.h"
#include "core/workflow/workflow.h"

extern MQTTService mqtt;

void mqttTick();
void onMqttPowerChanged(bool state);
void onMqttBrightnessChanged(uint8_t bri);