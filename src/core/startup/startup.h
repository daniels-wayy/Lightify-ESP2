#pragma once

#include <EEPROM.h>
#include <GyverPortal.h>
#include "misc/utils.h"
#include "misc/helpers.h"
#include "core/portal/portal_s.h"
#include "core/mqtt/mqtt_callback.h"
#include "core/eeprom/eeprom_ticker.h"
#include "core/mqtt/mqtt_ticker.h"
#include "ota_updater.h"
#include "core/ota/ota.h"
#include "led.h"
#include "timer.h"
#include "effects.h"
#include "config.h"
#include "mqtt.h"

extern GyverPortal* portal;
extern LedService led;
extern LedEffects effects;
extern Config cfg;
extern MQTTService mqtt;
extern OTAUpdater otaUpdater;

void attachPortal();
void startWiFi();
void startMQTT();
void setupOTA();
void checkPortalStart();