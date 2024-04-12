#include "mqtt_helpers.h"

void mqttTick()
{
    if (!mqtt.isConnected())
        mqtt.reconnect();
        
    mqtt.tick();
}