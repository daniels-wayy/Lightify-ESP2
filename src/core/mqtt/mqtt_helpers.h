#pragma once

#include <Arduino.h>
#include "mqtt.h"
#include "misc/utils.h"

extern MQTTService mqtt;

void mqttTick();
