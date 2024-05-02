#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "data.h"

struct Config
{
    char ssid[40] = "";
    char pass[32] = "";
    char local[30] = "";
    char remote[30] = MQTT_APP_TOPIC;
    char host[32] = MQTT_HOST;
    char apName[50] = "";
    char apPass[50] = "";
    uint16_t port = MQTT_PORT;
    uint8_t ip[4] = {0, 0, 0, 0};
    uint8_t btnPin = D5;
    bool useBtn = 0;
    bool usePortal = 0;
    bool power = 1;
    uint8_t brightness = 50;
    uint8_t colorHue = 32;
    uint8_t colorSaturation = 255;
    uint8_t colorValue = 255;
    uint8_t fxIndex = 0;
    uint16_t stripCurrent = STRIP_CURRENT_LIMIT;
    uint16_t ledCount = DEFAULT_LED_COUNT;
    uint8_t GMT = NTP_GMT;
    uint8_t workflowsCount = 0;

    Config() : 
        ssid(""), 
        pass(""),
        remote(MQTT_APP_TOPIC),
        host(MQTT_HOST),
        port(MQTT_PORT),
        ip{0, 0, 0, 0},
        btnPin(D5),
        useBtn(0),
        usePortal(0),
        power(1),
        brightness(50),
        colorHue(32),
        colorSaturation(255),
        colorValue(255),
        fxIndex(0),
        stripCurrent(STRIP_CURRENT_LIMIT),
        ledCount(DEFAULT_LED_COUNT),
        GMT(NTP_GMT),
        workflowsCount(0)
    {
        strcpy(local, DEVICE_CONFIG.localTopic);
        strcpy(apName, DEVICE_CONFIG.apName);
        strcpy(apPass, DEVICE_CONFIG.apPass);
    }

    inline CHSV currentColor() {
        return CHSV(
            colorHue,
            colorSaturation,
            power ? colorValue : 0
        );
    }
};

#define MODE_AMOUNT 4
struct ModeType
{
    uint8_t Speed;
    uint8_t Scale;
    ModeType() : Speed(45), Scale(127) {}
};

#define MAX_WORKFLOWS 10
struct Workflow
{
    bool isEnabled = false;
    uint8_t id = 0;         // id will be <day + hour + minute + duration>
    uint8_t day = 0;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t duration = 0;   // "dawn" or "sunset" duration, min
    uint8_t brightness = 0;   // "dawn" or "sunset" end brightness, 0...255s
    
    Workflow() : isEnabled(false), id(0), day(0), hour(0), minute(0), duration(0), brightness(0) {}

    inline bool isUnique() {
        return id > 0;
    }

    inline bool isMatchingDay(uint8_t currentDay) {
        return 
        /*everyday*/ day == WORKFLOW_EVERYDAY_NUM ||
        /*same day*/ day == currentDay ||
        /*every weekday*/ (day == WORKFLOW_EVERY_WEEKDAYS_NUM && currentDay > 0 && currentDay < 6) ||
        /*every weekend*/ (day == WORKFLOW_EVERY_WEEKENDS_NUM && (currentDay == 6 || currentDay == 0));
    }
};