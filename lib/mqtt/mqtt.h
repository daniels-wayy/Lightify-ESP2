#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <mString.h>
#include "config.h"
#include "timer.h"
#include "data.h"
#include "workflows.h"
#include "ota_updater.h"

class MQTTService
{
public:
    MQTTService(
        WiFiClient &client, 
        Config &cfg,
        ModeType *modeTypes,
        WorkflowsService &workflowsService,
        OTAUpdater &otaUpdater
    );
    PubSubClient *getClient();
    void setServerData();
    void onPacketReceived(char *data);
    void disconnect();
    bool connect();
    bool subscribe();
    bool isConnected();
    void tick();
    void reconnect();

    void setEECfgUpdate(void (*callback)()) { _EECfgUpdate = callback; }
    void setEECfgUpdateRst(void (*callback)()) { _EECfgUpdateRst = callback; }
    void setEEWorkflowsUpdate(void (*callback)()) { _EEWorkflowsUpdate = callback; }
    void setEEFxUpdate(void (*callback)()) { _EEFxUpdate = callback; }
    void setOnPowerChange(void (*callback)(bool)) { _onPowerChange = callback; }
    void setOnBrightnessChange(void (*callback)(uint8_t)) { _onBrightnessChange = callback; }
    void setOnFactoryReset(void (*callback)()) { _onFactoryReset = callback; }

private:
    PubSubClient _mqtt;
    Config *_cfg;
    ModeType *_modeTypes;
    WorkflowsService *_workflowsService;
    OTAUpdater &_otaUpdater;
    mString<MAX_MQTT_BUFFER_SIZE> _inputBuffer;
    mString<MAX_MQTT_BUFFER_SIZE> _outputBuffer;

    // callbacks
    void (*_EECfgUpdate)() = nullptr;
    void (*_EECfgUpdateRst)() = nullptr;
    void (*_EEWorkflowsUpdate)() = nullptr;
    void (*_EEFxUpdate)() = nullptr;
    void (*_onPowerChange)(bool) = nullptr;
    void (*_onBrightnessChange)(uint8_t) = nullptr;
    void (*_onFactoryReset)() = nullptr;

    void _parsePacket();
    void _onRequestedPowerChange();
    void _onRequestedBrightnessChange();
    void _onRequestedColorChange();
    void _onRequestedEffectChange();
    void _onRequestedEffectSpeedChange();
    void _onRequestedEffectScaleChange();
    void _onRequestedWorkflowAdd();
    void _onRequestedWorkflowUpdate();
    void _onRequestedWorkflowDelete();
    void _onRequestedWorkflowsClear();
    void _onRequestedSettingsUpdate();
    void _onRequestedSettingsReset();
    void _onRequestedCurrentState();
    void _onRequestedWorkflowsState();
    void _onRequestedSettingsState();
    void _onRequestedFirmwareUpdate();

    void _sendFirmwareUpdateStarted();
    void _sendFirmwareUpdateProgress(uint8_t percent);
    void _sendFirmwareUpdateFinished();
    void _sendFirmwareUpdateError(const char* errorMsg);
};