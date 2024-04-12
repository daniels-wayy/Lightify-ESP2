#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ntp_client.h"
#include "config.h"
#include "misc/utils.h"

extern NTPClient ntp;
extern Config cfg;
extern bool isTimeReady;

void setupTime();