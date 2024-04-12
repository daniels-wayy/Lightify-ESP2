#pragma once

#include <EEPROM.h>
#include <GyverPortal.h>
#include "misc/utils.h"
#include "core/portal/portal_s.h"
#include "core/mqtt/mqtt_callback.h"
#include "core/eeprom/eeprom_ticker.h"
#include "led.h"
#include "button.h"
#include "timer.h"
#include "effects.h"
#include "config.h"
#include "mqtt.h"

extern GyverPortal portal;
extern LedService led;
extern LedEffects effects;
extern Config cfg;
extern Button btn;
extern MQTTService mqtt;

void startSerial();
void attachPortal();
void startButton();
void startWiFi();
void startMQTT();
void startPortal();