#include "startup.h"

void attachPortal() {
    portal->attachBuild(buildWebInterface);
    portal->attach(checkPortal);
}

void startWiFi() {
    auto ip = IPAddress(cfg.ip[0], cfg.ip[1], cfg.ip[2], cfg.ip[3]);

    DEBUGLN(F("Connecting..."));
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(cfg.ssid, cfg.pass);

    Timer connectionTimer(WIFI_TOUT);
    while (WiFi.status() != WL_CONNECTED) {
        effects.loadingEffect(CRGB::Green);
        // if connection to WiFi is timed out
        if (connectionTimer.period()) {
            WiFi.disconnect();    // disconnect
            startLocalPortal(ip); // open portal
        }
    }

    led.clear();
    led.update();

    auto localIP = WiFi.localIP();

    DEBUG(F("Connected! IP: "));
    DEBUGLN(localIP);

    // store success IP to EEPROM
    if (ip != localIP) {
        ip = localIP;
        for (int i = 0; i < 4; i++) {
            cfg.ip[i] = ip[i];
        }
        EE_updateCfg();
    }
}

void startMQTT() {
    mqtt.setServerData();
    mqtt.getClient()->setCallback(mqttCallback);
    mqtt.setEECfgUpdate(EE_updateCfg);
    mqtt.setEECfgUpdateRst(EE_updateCfgRst);
    mqtt.setEEWorkflowsUpdate(EE_updateWorkflows);
    mqtt.setEEFxUpdate(EE_updateEffects);
    mqtt.setOnPowerChange(onMqttPowerChanged);
    mqtt.setOnBrightnessChange(onMqttBrightnessChanged);
    mqtt.setOnFactoryReset(factoryReset);

    if (mqtt.connect()) mqtt.subscribe();

    Timer connectionTimer(MQTT_TOUT);
    while (!mqtt.isConnected()) {
        effects.loadingEffect(CRGB::DarkRed);
        if (connectionTimer.period()) break;
    }
}

void setupOTA() {
    otaUpdater.setOnFirmwareUpdateStarted(onFirmwareUpdateStarted);
    otaUpdater.setOnFirmwareUpdateProgress(onFirmwareUpdateProgress);
    otaUpdater.setOnFirmwareUpdateFinished(onFirmwareUpdateFinished);
}

void checkPortalStart() {
    if (cfg.usePortal) {
        portal->start();
    }
    else {
        delete portal;
        portal = nullptr;
    }
}