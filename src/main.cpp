#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <GyverPortal.h>
#include <Udp.h>
#include "led.h"
#include "button.h"
#include "effects.h"
#include "config.h"
#include "workflows.h"
#include "mqtt.h"
#include "timer.h"
#include "time.h"
#include "ota_updater.h"
#include "ntp_client.h"
#include "misc/utils.h"
#include "core/startup/startup.h"
#include "core/portal/portal_s.h"
#include "core/mqtt/mqtt_ticker.h"
#include "core/led/led_ticker.h"
#include "core/effects/effects_ticker.h"
#include "core/eeprom/eeprom_ticker.h"
#include "core/eeprom/eeprom_setup.h"
#include "core/time/time_setup.h"
#include "core/time/time_ticker.h"
#include "core/workflow/workflow.h"
#include "core/button/button_ticker.h"
#include "core/ota/ota.h"

Timer EETimer(EE_TOUT, false);
bool EECfgFlag, EEWorkflowsFlag, EEFxFlag;
Timer workflowTickerTmr(WORKFLOW_UPDATE_FREQ, false), workflowPowerOnTmr, workflowPowerOffTmr;
bool isTimeReady;

Config cfg;
LedService led(cfg);
LedEffects effects(led, cfg);
Button btn(DEVICE_CONFIG.btnPin);
GyverPortal portal;
WiFiClient espClient;
ModeType modes[MODE_AMOUNT];
Workflow workflows[MAX_WORKFLOWS];
Time now;
WorkflowsService workflowsService(cfg, workflows, now);
WiFiUDP ntpUdp;
NTPClient ntp(ntpUdp);
OTAUpdater otaUpdater;
MQTTService mqtt(
      espClient,
      cfg,
      modes,
      workflowsService,
      otaUpdater
);

void setup() {
  delay(2000);
  startSerial();

  if (ERASE_EEPROM) {
    EE_erase();
    return;
  }

  attachPortal();
  EE_setup();
  led.initialize();
  startButton();
  startWiFi();
  startMQTT();
  setupOTA();
  randomSeed(micros());
  startPortal();
  setupTime();
  led.setBrightness(cfg.brightness);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    effects.loadingEffect(CRGB::WhiteSmoke);
    return;
  }
  timeTicker();
  yield();
  mqttTick();
  yield();
  workflowTicker();
  checkWorkflowTimers();
  yield();
  EE_ticker();
  ledTick();
  effectsTicker();
  yield();
  portal.tick();
  checkPortal();
  buttonTicker();
  yield();
}