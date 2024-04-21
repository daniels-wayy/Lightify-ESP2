#include "mqtt_callback.h"

void mqttCallback(char *topic, byte *payload, uint16_t len) {
    payload[len] = '\0';
    char *str = (char *)payload;

    // not our packet
    if (strncmp(str, MQTT_HEADER, hLen))
        return;

    str += hLen; // header shift
    mqtt.onPacketReceived(str);
}
