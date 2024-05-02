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
#include "misc/misc.h"
#include "core/startup/startup.h"
#include "core/portal/portal_s.h"
#include "core/mqtt/mqtt_ticker.h"
#include "core/led/led_ticker.h"
#include "core/eeprom/eeprom_ticker.h"
#include "core/eeprom/eeprom_setup.h"
#include "core/time/time_setup.h"
#include "core/time/time_ticker.h"
#include "core/workflow/workflow.h"
#include "core/workflow/workflow_ticker.h"
#include "core/button/button_ticker.h"
#include "core/ota/ota.h"

Timer EETimer(EE_TOUT, false);
bool EECfgFlag, EEWorkflowsFlag, EEFxFlag;
Timer workflowTimer(WORKFLOW_UPDATE_FREQ, false), dawnTimer, sunsetTimer;
uint8_t dawnInitialBrightness, sunsetInitialBrightness;
Workflow* currentWorkflow = nullptr;
bool isTimeReady;
Config cfg;
LedService led(cfg);
LedEffects effects(led, cfg);
Button* button = nullptr;
GyverPortal* portal = nullptr;
WiFiClient espClient;
ModeType modes[MODE_AMOUNT];
Workflow workflows[MAX_WORKFLOWS];
Time now;
WorkflowsService workflowsService(cfg, workflows, now);
WiFiUDP ntpUdp;
NTPClient ntp(ntpUdp);
OTAUpdater otaUpdater(led);
MQTTService mqtt(
      espClient,
      cfg,
      modes,
      workflowsService,
      otaUpdater
);
const uint16_t eeSize = 
  sizeof(cfg) + 
  sizeof(workflows) + 
  sizeof(modes) + 2;

void setup() {
  delay(2000);
  misc();
  EE_setup();
  led.initialize();
  if (cfg.useBtn) {
    button = new Button(cfg.btnPin);
    button->setLevel(digitalRead(cfg.btnPin));
  }
  if (isDebugDevice(cfg.local)) Serial.begin(115200);
  portal = new GyverPortal();
  attachPortal();
  startWiFi();
  setupOTA();
  setupTime();
  randomSeed(micros());
  led.setBrightness(cfg.brightness);
  startMQTT();
  checkPortalStart();
}

void loop() {
  timeTicker();
  yield();
  mqttTick();
  checkWorkflow();
  yield();
  workflowTick();
  ledTick();
  yield();
  EE_ticker();
  if (cfg.useBtn) buttonTicker(button);
  if (cfg.usePortal) {
    portal->tick();
    checkPortal();
  }
  ESP.wdtFeed();
  yield();
}