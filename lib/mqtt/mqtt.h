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
        OTAUpdater &otaUpdater,
        bool &fxPropertyChanged,
        Timer &workflowPowerOnTmr,
        Timer &workflowPowerOffTmr);
    void setServerData();
    PubSubClient *getClient();
    void onPacketReceived(
        char *data,
        void (*EEUpdate)(),
        void (*EECfgUpdateNow)(),
        void (*EEWorkflowUpdate)(),
        void (*EEEffectsUpdate)(),
        void (*stopWorkflow)());
    void disconnect();
    bool connect();
    bool subscribe();
    bool isConnected();
    void tick();
    void reconnect();

    void _sendFirmwareUpdateStarted();
    void _sendFirmwareUpdateProgress(uint8_t percent);
    void _sendFirmwareUpdateFinished();
    void _sendFirmwareUpdateError(const char* errorMsg);

private:
    PubSubClient _mqtt;
    Config *_cfg;
    ModeType *_modeTypes;
    WorkflowsService *_workflowsService;
    OTAUpdater *_otaUpdater;
    bool &_fxPropertyChanged;
    Timer &_workflowPowerOnTmr;
    Timer &_workflowPowerOffTmr;
    mString<MAX_MQTT_BUFFER_SIZE> _inputBuffer;
    mString<MAX_MQTT_BUFFER_SIZE> _outputBuffer;
    bool _isSending = false;
    bool _isFirmwareUpdateCalled = false;

    void _parsePacket(
        void (*EECfgUpdate)(),
        void (*EECfgUpdateNow)(),
        void (*EEWorkflowUpdate)(),
        void (*EEEffectsUpdate)(),
        void (*stopWorkflow)());
    void _sendCurrentState();
    void _sendWorkflowsState();
    void _sendSettingsState();
};