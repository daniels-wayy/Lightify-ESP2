#include "mqtt.h"

MQTTService::MQTTService(
    WiFiClient &client,
    Config &cfg,
    ModeType *modeTypes,
    WorkflowsService &workflowsService,
    OTAUpdater &otaUpdater,
    bool &fxPropertyChanged,
    Timer &workflowPowerOnTmr,
    Timer &workflowPowerOffTmr)
    : _mqtt(client),
      _cfg(&cfg),
      _modeTypes(modeTypes),
      _workflowsService(&workflowsService),
      _otaUpdater(&otaUpdater),
      _fxPropertyChanged(fxPropertyChanged),
      _workflowPowerOnTmr(workflowPowerOnTmr),
      _workflowPowerOffTmr(workflowPowerOffTmr)
{
}

void MQTTService::setServerData()
{
    _mqtt.setServer(_cfg->host, _cfg->port);
}

void MQTTService::disconnect()
{
    _mqtt.disconnect();
}

bool MQTTService::connect()
{
    String id("WebLightifier-");
    id += String(random(0xffffff), HEX);
    return _mqtt.connect(id.c_str());
}

bool MQTTService::subscribe()
{
    return _mqtt.subscribe(_cfg->local);
}

bool MQTTService::isConnected()
{
    return _mqtt.connected();
}

void MQTTService::tick()
{
    _mqtt.loop();
}

void MQTTService::reconnect()
{
    if (!isConnected()) {
        if (USE_SERIAL) {
            Serial.println("MQTT dropped off. Reconnecting...");
        }
        if (connect()) 
            if (subscribe()) {
                if (USE_SERIAL) {
                    Serial.println("MQTT successfully connected.");
                }
            }

        delay(1000);
    }
}

PubSubClient *MQTTService::getClient()
{
    return &_mqtt;
}

void MQTTService::onPacketReceived(
    char *data,
    void (*EECfgUpdate)(),
    void (*EECfgUpdateNow)(),
    void (*EEWorkflowUpdate)(),
    void (*EEEffectsUpdate)(),
    void (*stopWorkflow)())
{
    _inputBuffer.clear();
    _inputBuffer.add(data);
    _parsePacket(
        EECfgUpdate,
        EECfgUpdateNow,
        EEWorkflowUpdate,
        EEEffectsUpdate,
        stopWorkflow);
}

void MQTTService::_parsePacket(
    void (*EECfgUpdate)(),
    void (*EECfgUpdateNow)(),
    void (*EEWorkflowUpdate)(),
    void (*EEEffectsUpdate)(),
    void (*stopWorkflow)())
{
    if (_inputBuffer.startsWith(GET_CMD))
    {
        // get device state
        _sendCurrentState();
    }
    else if (_inputBuffer.startsWith(WFL_CMD))
    {
        // get workflows state
        _sendWorkflowsState();
    }
    else if (_inputBuffer.startsWith(STG_CMD))
    {
        // get settings state
        _sendSettingsState();
    }
    else if (_inputBuffer.startsWith(PWR_CMD))
    {
        stopWorkflow();
        uint8_t powerState = _inputBuffer.toInt(3);
        _cfg->power = powerState;
        if (_cfg->brightness == 0)
        {
            _cfg->brightness = 10;
        }
        EECfgUpdate();
    }
    else if (_inputBuffer.startsWith(BRI_CMD))
    {
        stopWorkflow();
        uint8_t brightness = _inputBuffer.toInt(3);
        uint8_t constrainedBrightness = constrain(brightness, 0, 255);
        _cfg->brightness = constrainedBrightness;
        EECfgUpdate();
    }
    else if (_inputBuffer.startsWith(CLR_CMD))
    {
        // CLRhue,sat,val
        byte colorData[10];
        _inputBuffer.remove(0, 3);
        _inputBuffer.parse(colorData, 1, 10);

        uint8_t colorHue = constrain(colorData[0], 0, 255);
        uint8_t colorSat = constrain(colorData[1], 0, 255);
        uint8_t colorVal = constrain(colorData[2], 0, 255);

        _cfg->colorHue = colorHue;
        _cfg->colorSaturation = colorSat;
        _cfg->colorValue = colorVal;

        EECfgUpdate();
    }
    else if (_inputBuffer.startsWith(EFF_CMD))
    {
        uint8_t effectIndex = _inputBuffer.toInt(3);
        _cfg->fxIndex = effectIndex;
        _fxPropertyChanged = true;
        EECfgUpdate();
    }
    else if (_inputBuffer.startsWith(SPD_CMD))
    {
        uint8_t speed = _inputBuffer.toInt(3);
        _modeTypes[_cfg->fxIndex].Speed = speed;
        _fxPropertyChanged = true;
        EEEffectsUpdate();
    }
    else if (_inputBuffer.startsWith(SCL_CMD))
    {
        uint8_t scale = _inputBuffer.toInt(3);
        _modeTypes[_cfg->fxIndex].Scale = scale;
        _fxPropertyChanged = true;
        EEEffectsUpdate();
    }
    else if (_inputBuffer.startsWith(WFL_ADD_CMD))
    {
        // add workflow
        if (_workflowsService->isAbleToAdd())
        {
            uint8_t _workflowData[10];
            _inputBuffer.remove(0, 5);
            _inputBuffer.parse(_workflowData, 1, 10);
            _workflowsService->add(
                _workflowData[0],
                _workflowData[1],
                _workflowData[2],
                _workflowData[3],
                _workflowData[4],
                _workflowData[5],
                _workflowData[6],
                _workflowData[7]);
            EECfgUpdate();
            EEWorkflowUpdate();
        }
    }
    else if (_inputBuffer.startsWith(WFL_DEL_CMD))
    {
        // delete workflow
        uint8_t _workflowData[1];
        _inputBuffer.remove(0, 5);
        _inputBuffer.parse(_workflowData, 1, 1);
        _workflowsService->del(_workflowData[0]);
        EECfgUpdate();
        EEWorkflowUpdate();
    }
    else if (_inputBuffer.startsWith(WFL_UPD_CMD))
    {
        // update workflow
        uint8_t _workflowData[10];
        _inputBuffer.remove(0, 5);
        _inputBuffer.parse(_workflowData, 1, 10);
        _workflowsService->update(
            _workflowData[0],
            _workflowData[1],
            _workflowData[2],
            _workflowData[3],
            _workflowData[4],
            _workflowData[5],
            _workflowData[6],
            _workflowData[7]);
        EECfgUpdate();
        EEWorkflowUpdate();
    }
    else if (_inputBuffer.startsWith(STG_UPD_CMD))
    {

        uint16_t data[4];
        _inputBuffer.remove(0, 5);
        _inputBuffer.parse(data, 2, 4);

        uint16_t port = data[0];
        uint16_t curr = (uint16_t)data[1];
        uint16_t led = data[2];
        uint16_t gmt = data[3];

        _cfg->port = port;
        _cfg->stripCurrent = curr;
        _cfg->ledCount = led;
        _cfg->GMT = gmt;

        EECfgUpdateNow();
    }
    else if (!_isFirmwareUpdateCalled && _inputBuffer.startsWith(FWR_UPD_CMD))
    {
        _isFirmwareUpdateCalled = true;
        _inputBuffer.remove(0, 5);
        _otaUpdater->update(
            _inputBuffer.buf,
            [this](){_sendFirmwareUpdateStarted();},
            [this](uint8_t percent){_sendFirmwareUpdateProgress(percent);},
            [this](){_sendFirmwareUpdateFinished();},
            [this](const char *errorMsg){_sendFirmwareUpdateError(errorMsg);}
        );
    }
}

void MQTTService::_sendCurrentState()
{
    if (_isSending)
    {
        return;
    }
    _isSending = true;
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
    _isSending = false;
}

void MQTTService::_sendWorkflowsState()
{
    if (_isSending)
    {
        return;
    }
    _isSending = true;
    _outputBuffer.clear();

    _outputBuffer = MQTT_HEADER;
    _outputBuffer += WFL_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += "/";

    for (uint8_t i = 0; i < _cfg->workflowsCount; i++)
    {
        _outputBuffer += _workflowsService->getWorkflows()[i].id;
        _outputBuffer += ",";
        _outputBuffer += _workflowsService->getWorkflows()[i].isEnabled;
        _outputBuffer += ",";
        _outputBuffer += _workflowsService->getWorkflows()[i].isPowerOn;
        _outputBuffer += ",";
        _outputBuffer += _workflowsService->getWorkflows()[i].day;
        _outputBuffer += ",";
        _outputBuffer += _workflowsService->getWorkflows()[i].hour;
        _outputBuffer += ",";
        _outputBuffer += _workflowsService->getWorkflows()[i].minute;
        _outputBuffer += ",";
        _outputBuffer += _workflowsService->getWorkflows()[i].duration;
        _outputBuffer += ",";
        _outputBuffer += _workflowsService->getWorkflows()[i].brightness;

        if (i < _cfg->workflowsCount - 1)
        {
            // add divider if is not last
            _outputBuffer += ";";
        }
    }
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
    _isSending = false;
}

void MQTTService::_sendSettingsState()
{
    if (_isSending)
    {
        return;
    }
    _isSending = true;
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
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
    _isSending = false;
}

void MQTTService::_sendFirmwareUpdateStarted()
{
    if (_isSending)
    {
        return;
    }
    _isSending = true;
    _outputBuffer.clear();

    if (USE_SERIAL) {
        Serial.println("Firmware update started!");
    }

    _outputBuffer = MQTT_HEADER;
    _outputBuffer += FMW_UPDATE_STATUS_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += UPD_STARTED_KEY;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
    _isSending = false;
}

void MQTTService::_sendFirmwareUpdateProgress(uint8_t percent)
{
    if (_isSending)
    {
        return;
    }

    reconnect(); // 1st try

    if (!isConnected()) {
        reconnect(); // 2nd try
    }

    _isSending = true;
    _outputBuffer.clear();

    if (USE_SERIAL) {
        Serial.print("Firmware update progress: ");
        Serial.print(percent);
        Serial.println("%");
    }

    _outputBuffer = MQTT_HEADER;
    _outputBuffer += FMW_UPDATE_STATUS_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += UPD_PROGRESS_KEY;
    _outputBuffer += ",";
    _outputBuffer += percent;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
    _isSending = false;
}

void MQTTService::_sendFirmwareUpdateFinished()
{
    _isFirmwareUpdateCalled = false;

    if (_isSending)
    {
        return;
    }
    _isSending = true;
    _outputBuffer.clear();

    if (USE_SERIAL) {
        Serial.println("Firmware update finished!");
    }

    _outputBuffer = MQTT_HEADER;
    _outputBuffer += FMW_UPDATE_STATUS_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += UPD_FINISHED_KEY;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
    _mqtt.loop();
    _isSending = false;

    delay(100);
}

void MQTTService::_sendFirmwareUpdateError(const char* errorMsg)
{
    _isFirmwareUpdateCalled = false;

    if (_isSending)
    {
        return;
    }
    _isSending = true;
    _outputBuffer.clear();

    if (USE_SERIAL) {
        Serial.print("Firmware update error: ");
        Serial.println(errorMsg);
    }

    _outputBuffer = MQTT_HEADER;
    _outputBuffer += FMW_UPDATE_STATUS_HEADER;
    _outputBuffer += _cfg->local;
    _outputBuffer += ",";
    _outputBuffer += UPD_ERROR_KEY;
    _outputBuffer += ",";
    _outputBuffer += errorMsg;
    _mqtt.publish(_cfg->remote, _outputBuffer.buf);
    _mqtt.loop();
    _isSending = false;
}