#include "mqtt.h"

MQTTService::MQTTService(
    WiFiClient &client,
    Config &cfg,
    ModeType *modeTypes,
    WorkflowsService &workflowsService,
    OTAUpdater &otaUpdater)
    : _mqtt(client),
      _cfg(&cfg),
      _modeTypes(modeTypes),
      _workflowsService(&workflowsService),
      _otaUpdater(otaUpdater) {}

void MQTTService::setServerData() {
    _mqtt.setServer(_cfg->host, _cfg->port);
}

void MQTTService::disconnect() {
    _mqtt.disconnect();
}

bool MQTTService::connect() {
    String id("WebLightifier-");
    id += String(random(0xffffff), HEX);
    return _mqtt.connect(id.c_str());
}

bool MQTTService::subscribe() {
    return _mqtt.subscribe(_cfg->local);
}

bool MQTTService::isConnected() {
    return _mqtt.connected();
}

void MQTTService::tick() {
    _mqtt.loop();
}

void MQTTService::reconnect() {
    if (!isConnected()) {
        if (connect()) subscribe();
        delay(1200);
    }
}

PubSubClient *MQTTService::getClient() {
    return &_mqtt;
}

void MQTTService::onPacketReceived(char *data) {
    _inputBuffer.clear();
    _inputBuffer.add(data);
    _parsePacket();
}

void MQTTService::_parsePacket() {
    if (_inputBuffer.startsWith(GET_CMD)) { _onRequestedCurrentState(); }
    else if (_inputBuffer.startsWith(WFL_CMD)) { _onRequestedWorkflowsState(); }
    else if (_inputBuffer.startsWith(STG_CMD)) { _onRequestedSettingsState(); }
    else if (_inputBuffer.startsWith(PWR_CMD)) { _onRequestedPowerChange(); }
    else if (_inputBuffer.startsWith(BRI_CMD)) { _onRequestedBrightnessChange(); }
    else if (_inputBuffer.startsWith(CLR_CMD)) { _onRequestedColorChange(); }
    else if (_inputBuffer.startsWith(EFF_CMD)) { _onRequestedEffectChange(); }
    else if (_inputBuffer.startsWith(SPD_CMD)) { _onRequestedEffectSpeedChange(); }
    else if (_inputBuffer.startsWith(SCL_CMD)) { _onRequestedEffectScaleChange(); }
    else if (_inputBuffer.startsWith(WFL_ADD_CMD)) { _onRequestedWorkflowAdd(); }
    else if (_inputBuffer.startsWith(WFL_UPD_CMD)) { _onRequestedWorkflowUpdate(); }
    else if (_inputBuffer.startsWith(WFL_DEL_CMD)) { _onRequestedWorkflowDelete(); }
    else if (_inputBuffer.startsWith(WFL_CLR_CMD)) { _onRequestedWorkflowsClear(); }
    else if (_inputBuffer.startsWith(STG_UPD_CMD)) { _onRequestedSettingsUpdate(); }
    else if (_inputBuffer.startsWith(STG_RST_CMD)) { _onRequestedSettingsReset(); }
    else if (_inputBuffer.startsWith(FWR_UPD_CMD)) { _onRequestedFirmwareUpdate(); }
}

void MQTTService::_onRequestedPowerChange() {
    bool powerState = _inputBuffer.toInt(3);
    _cfg->power = powerState;
    if (_cfg->brightness == 0) _cfg->brightness = BRIGHTNESS_CHANGE_MIN;
    if (_EECfgUpdate) _EECfgUpdate();
    if (_onPowerChange) _onPowerChange(powerState);
}

void MQTTService::_onRequestedBrightnessChange() {
    uint8_t brightness = _inputBuffer.toInt(3);
    uint8_t constrainedBrightness = constrain(brightness, BRIGHTNESS_CHANGE_MIN, BRIGHTNESS_CHANGE_MAX);
    _cfg->brightness = constrainedBrightness;
    if (_EECfgUpdate) _EECfgUpdate();
    if (_onBrightnessChange) _onBrightnessChange(constrainedBrightness);
}

void MQTTService::_onRequestedColorChange() {
    uint8_t colorData[10];
    _inputBuffer.remove(0, 3);
    _inputBuffer.parse(colorData, 1, 10);
    uint8_t colorHue = constrain(colorData[0], 0, 255);
    uint8_t colorSat = constrain(colorData[1], 0, 255);
    uint8_t colorVal = constrain(colorData[2], 0, 255);
    _cfg->colorHue = colorHue;
    _cfg->colorSaturation = colorSat;
    _cfg->colorValue = colorVal;
    if (_EECfgUpdate) _EECfgUpdate();
}

void MQTTService::_onRequestedEffectChange() {
    uint8_t effectIndex = _inputBuffer.toInt(3);
    _cfg->fxIndex = effectIndex;
    if (_EECfgUpdate) _EECfgUpdate();
}

void MQTTService::_onRequestedEffectSpeedChange() {
    uint8_t speed = _inputBuffer.toInt(3);
    _modeTypes[_cfg->fxIndex].Speed = speed;
    if (_EEFxUpdate) _EEFxUpdate();
}

void MQTTService::_onRequestedEffectScaleChange() {
    uint8_t scale = _inputBuffer.toInt(3);
    _modeTypes[_cfg->fxIndex].Scale = scale;
    if (_EEFxUpdate) _EEFxUpdate();
}

void MQTTService::_onRequestedWorkflowAdd() {
    uint8_t _workflowData[20];
    _inputBuffer.remove(0, 5);
    _inputBuffer.parse(_workflowData, 1, 20);
    _workflowsService->add(
        _workflowData[0],
        _workflowData[1],
        _workflowData[2],
        _workflowData[3],
        _workflowData[4],
        _workflowData[5],
        _workflowData[6]);
    if (_EECfgUpdate) _EECfgUpdate();
    if (_EEWorkflowsUpdate) _EEWorkflowsUpdate();
}

void MQTTService::_onRequestedWorkflowUpdate() {
    uint8_t _workflowData[20];
    _inputBuffer.remove(0, 5);
    _inputBuffer.parse(_workflowData, 1, 20);
    _workflowsService->update(
        _workflowData[0],
        _workflowData[1],
        _workflowData[2],
        _workflowData[3],
        _workflowData[4],
        _workflowData[5],
        _workflowData[6]);
    if (_EECfgUpdate) _EECfgUpdate();
    if (_EEWorkflowsUpdate) _EEWorkflowsUpdate();
}

void MQTTService::_onRequestedWorkflowDelete() {
    uint8_t _workflowData[1];
    _inputBuffer.remove(0, 5);
    _inputBuffer.parse(_workflowData, 1, 1);
    _workflowsService->del(_workflowData[0]);
    if (_EECfgUpdate) _EECfgUpdate();
    if (_EEWorkflowsUpdate) _EEWorkflowsUpdate();
}

void MQTTService::_onRequestedWorkflowsClear() {
    _workflowsService->clear();
    if (_EECfgUpdate) _EECfgUpdate();
    if (_EEWorkflowsUpdate) _EEWorkflowsUpdate();
}

void MQTTService::_onRequestedSettingsUpdate() {
    uint16_t data[5];
    _inputBuffer.remove(0, 5);
    _inputBuffer.parse(data, 2, 5);
    _cfg->port = data[0];
    _cfg->stripCurrent = (uint16_t)data[1];
    _cfg->ledCount = data[2];
    _cfg->GMT = data[3];
    _cfg->usePortal = bool(data[4]);
    if (_EECfgUpdateRst) {
        _EECfgUpdateRst();
    }
}

void MQTTService::_onRequestedSettingsReset() {
    if (_onFactoryReset) _onFactoryReset();
}

void MQTTService::_onRequestedCurrentState() {
    _outputBuffer.clear();
    _outputBuffer = MQTT_HEADER;
    _outputBuffer += DEV_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += _cfg->power;
    _outputBuffer += ",";
    _outputBuffer += _cfg->brightness;
    _outputBuffer += ",";
    _outputBuffer += _cfg->colorHue;
    _outputBuffer += ",";
    _outputBuffer += _cfg->colorSaturation;
    _outputBuffer += ",";
    _outputBuffer += _cfg->colorValue;
    _outputBuffer += ",";
    _outputBuffer += _cfg->fxIndex;
    _outputBuffer += ",";
    _outputBuffer += _modeTypes[_cfg->fxIndex].Speed;
    _outputBuffer += ",";
    _outputBuffer += _modeTypes[_cfg->fxIndex].Scale;
    _outputBuffer += ",";
    _outputBuffer += LY_VERSION;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
}

void MQTTService::_onRequestedWorkflowsState() {
    _outputBuffer.clear();
    _outputBuffer = MQTT_HEADER;
    _outputBuffer += WFL_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += "/";
    for (uint8_t i = 0; i < MAX_WORKFLOWS; i++) {
        auto _workflow = &_workflowsService->getWorkflows()[i];
        if (_workflow->id <= 0) continue;
        _outputBuffer += _workflow->id;
        _outputBuffer += ",";
        _outputBuffer += _workflow->isEnabled;
        _outputBuffer += ",";
        _outputBuffer += _workflow->day;
        _outputBuffer += ",";
        _outputBuffer += _workflow->hour;
        _outputBuffer += ",";
        _outputBuffer += _workflow->minute;
        _outputBuffer += ",";
        _outputBuffer += _workflow->duration;
        _outputBuffer += ",";
        _outputBuffer += _workflow->brightness;

        if (i < _cfg->workflowsCount - 1) {
            _outputBuffer += ";"; // add divider if is not last
        }
    }
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
}

void MQTTService::_onRequestedSettingsState() {
    _outputBuffer.clear();
    _outputBuffer = MQTT_HEADER;
    _outputBuffer += STG_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += _cfg->port,
    _outputBuffer += ",";
    _outputBuffer += _cfg->stripCurrent,
    _outputBuffer += ",";
    _outputBuffer += _cfg->ledCount,
    _outputBuffer += ",";
    _outputBuffer += _cfg->GMT,
    _outputBuffer += ",";
    _outputBuffer += _cfg->ip[0];
    _outputBuffer += ",";
    _outputBuffer += _cfg->ip[1];
    _outputBuffer += ",";
    _outputBuffer += _cfg->ip[2];
    _outputBuffer += ",";
    _outputBuffer += _cfg->ip[3];
    _outputBuffer += ",";
    _outputBuffer += _cfg->usePortal;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
}

void MQTTService::_onRequestedFirmwareUpdate() {
    _inputBuffer.remove(0, 5);
    _otaUpdater.update(
        _inputBuffer.buf,
        [this]() {
            this->_sendFirmwareUpdateStarted();
        },
        [this](uint8_t percent) {
            this->_sendFirmwareUpdateProgress(percent);
        },
        [this]() {
            this->_sendFirmwareUpdateFinished();
        },
        [this](const char *errorMsg) {
            this->_sendFirmwareUpdateError(errorMsg);
        }
    );
}

void MQTTService::_sendFirmwareUpdateStarted() {
    _outputBuffer.clear();
    _outputBuffer = MQTT_HEADER;
    _outputBuffer += FMW_UPDATE_STATUS_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += UPD_STARTED_KEY;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
}

void MQTTService::_sendFirmwareUpdateProgress(uint8_t percent) {
    reconnect(); // 1st try
    reconnect(); // 2nd try
    _outputBuffer.clear();
    _outputBuffer = MQTT_HEADER;
    _outputBuffer += FMW_UPDATE_STATUS_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += UPD_PROGRESS_KEY;
    _outputBuffer += ",";
    _outputBuffer += percent;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
}

void MQTTService::_sendFirmwareUpdateFinished() {
    _outputBuffer.clear();
    _outputBuffer = MQTT_HEADER;
    _outputBuffer += FMW_UPDATE_STATUS_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += UPD_FINISHED_KEY;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
    _mqtt.loop();
    delay(100);
}

void MQTTService::_sendFirmwareUpdateError(const char* errorMsg) {
    _outputBuffer.clear();
    _outputBuffer = MQTT_HEADER;
    _outputBuffer += FMW_UPDATE_STATUS_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += UPD_ERROR_KEY;
    _outputBuffer += ",";
    _outputBuffer += errorMsg;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
    _mqtt.loop();
}