#pragma once

#include <Arduino.h>
#include "ntp_client.h"
#include "config.h"
#include "timer.h"
#include "time.h"
#include "misc/utils.h"

extern NTPClient ntp;
extern Config cfg;
extern bool isTimeReady;
extern Time now;

void timeTicker();