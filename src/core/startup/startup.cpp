#include "startup.h"

void startSerial()
{
    if (USE_SERIAL)
    {
        Serial.begin(SERIAL_BAUD);
        DEBUGLN();
    }
}

void attachPortal()
{
    if (USE_PORTAL)
    {
        portal.attachBuild(buildWebInterface);
        portal.attach(checkPortal);
    }
}

void startButton()
{
    if (DEVICE_CONFIG.useBtn)
    {
        btn.setLevel(digitalRead(DEVICE_CONFIG.btnPin));
    }
}

void startWiFi()
{
    auto ip = IPAddress(cfg.ip[0], cfg.ip[1], cfg.ip[2], cfg.ip[3]);

    DEBUGLN(F("Connecting..."));
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(cfg.ssid, cfg.pass);

    Timer connectionTimer(WIFI_TOUT);
    while (WiFi.status() != WL_CONNECTED)
    {
        effects.loadingEffect(CRGB::Green);
        if (DEVICE_CONFIG.useBtn)
        {
            btn.tick();
        }

        // if button is tapped or connection to router is timed out
        if ((DEVICE_CONFIG.useBtn && btn.isClick()) || connectionTimer.period())
        {
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
    if (ip != localIP)
    {
        ip = localIP;
        for (int i = 0; i < 4; i++) {
            cfg.ip[i] = ip[i];
        }
        EE_updateCfg();
    }
}

void startMQTT()
{
    mqtt.setServerData();
    mqtt.getClient()->setCallback(mqttCallback);
}

void startPortal()
{
    if (USE_PORTAL)
    {
        portal.start();
    }
}