#include "mqtt_ticker.h"

void mqttTick() {
    mqtt.reconnect(); // check if need to reconnect 
    mqtt.tick();
}

void onMqttPowerChanged(bool state) {
    if (isWorkflowRunning()) {
        stopWorkflowProcess();
    }
}

void onMqttBrightnessChanged(uint8_t bri) {
    if (isWorkflowRunning()) {
        stopWorkflowProcess();
    }
}