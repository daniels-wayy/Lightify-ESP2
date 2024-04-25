#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "data.h"

struct Config
{
    char ssid[40] = "";
    char pass[32] = "";
    char local[30] = "";
    char remote[30] = "DSLY_App";
    char host[32] = "broker.mqttdashboard.com";
    char apName[50] = "";
    char apPass[50] = "";
    uint16_t port = 1883;
    uint8_t ip[4] = {0, 0, 0, 0};
    uint8_t ledPin = DEVICE_CONFIG.ledPin;
    uint8_t btnPin = DEVICE_CONFIG.btnPin;
    bool useBtn = DEVICE_CONFIG.useBtn;
    bool power = 1;
    uint8_t brightness = 50;
    uint8_t colorHue = 64;
    uint8_t colorSaturation = 255;
    uint8_t colorValue = 255;
    uint8_t fxIndex = 0;
    uint16_t stripCurrent = DEVICE_CONFIG.stripCurrent;
    uint16_t ledCount = DEVICE_CONFIG.ledCount;
    uint8_t GMT = 3;
    uint8_t workflowsCount = 0;

    Config() {
        strcpy(local, DEVICE_CONFIG.localTopic);
        strcpy(apName, DEVICE_CONFIG.apName);
        strcpy(apPass, DEVICE_CONFIG.apPass);
    }

    inline CHSV currentColor() {
        return CHSV(colorHue, colorSaturation, power ? colorValue : 0);
    }
};

#define MODE_AMOUNT 4
struct ModeType
{
    uint8_t Speed = 4;
    uint8_t Scale = 8;
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
};