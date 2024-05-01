#pragma once

#include "device_type.h"

#define LY_VERSION 2.4 // firmware version

constexpr auto DEVICE_TYPE = debug;
constexpr auto DEVICE_CONFIG = getNetworkConfig(DEVICE_TYPE);
constexpr bool isDebugDevice(char *localTopic) {
    return strncmp(localTopic, getNetworkConfig(debug).localTopic, 10) == 0;
}

// *** STRIP ***
#define STRIP_PIN D7
#define MAX_LEDS 400
#define STRIP_VOLT 5
#define STRIP_CURRENT_LIMIT 3000 // mAh
#define DEFAULT_LED_COUNT 3 // can be adjusted later in the settings
#define MAX_LOAD_ANIM_BRI 80 // strip - animation loading effect pulse max brightness
#define MIN_LOAD_ANIM_BRI 20 // strip - animation loading effect pulse min brightness
#define BRIGHTNESS_CHANGE_STEP 5
#define BRIGHTNESS_CHANGE_MIN 10
#define BRIGHTNESS_CHANGE_MAX 255

// *** WORKFLOWS ***
#define WORKFLOW_UPDATE_FREQ 2500 // timer frequency to change power state set in schedule, sec
#define WORKFLOW_EVERY_WEEKDAYS_NUM 7
#define WORKFLOW_EVERY_WEEKENDS_NUM 8
#define WORKFLOW_EVERYDAY_NUM 9
#define WORKFLOW_MAX_DURATION 90 // min

// *** NTP ***
#define NTP_UPD_PRD 5 // time sync, min
#define NTP_GMT 3 // timezone shift, hours
const char NTPserver[] = "pool.ntp.org";

// *** MQTT ***
#define MQTT_HEADER "DSLY:"         // packets header
#define MQTT_APP_TOPIC "DSLY_App"
#define MQTT_HOST "broker.mqttdashboard.com"
#define MQTT_PORT 1883
#define MAX_MQTT_BUFFER_SIZE (255U) // MQTT max buffer size
const uint8_t hLen = strlen(MQTT_HEADER);

#define GET_CMD "GET"                   // Get device state (Requests from the client app)
#define WFL_CMD "WFLGT"                 // Get device workflows (Requests from the client app)
#define STG_CMD "STGGT"                 // Get device settings (Requests from the client app)
#define PWR_CMD "PWR"                   // Set device power state
#define BRI_CMD "BRI"                   // Set device brightness state
#define CLR_CMD "CLR"                   // Set device color state
#define EFF_CMD "EFF"                   // Set device effect index
#define SPD_CMD "SPD"                   // Set device effect speed
#define SCL_CMD "SCL"                   // Set device effect scale
#define WFL_ADD_CMD "WFLDD"             // Add workflow item
#define WFL_DEL_CMD "WFLEL"             // Remove workflow item
#define WFL_CLR_CMD "WFLCL"             // Clear workflows
#define WFL_UPD_CMD "WFLPD"             // Update workflow item
#define STG_UPD_CMD "STGPD"             // Update device settings
#define STG_RST_CMD "STGRT"             // Reset device settings
#define FWR_UPD_CMD "FWRPD"             // Update device firmware
#define DEV_HEADER "DEV"                // reply header for get device state
#define WFL_HEADER "WFLS"               // reply header for get device workflows
#define STG_HEADER "STGS"               // reply header for get device settings
#define FMW_UPDATE_STATUS_HEADER "FWST" // reply header for get device update
#define UPD_STARTED_KEY "ST"
#define UPD_PROGRESS_KEY "PG"
#define UPD_FINISHED_KEY "FD"
#define UPD_ERROR_KEY "ER"

// *** MISC ***
#define EE_TOUT 30000   // timeout for update EEPROM, ms
#define WIFI_TOUT 90000 // timeout while trying to connect to wifi , ms
#define MQTT_TOUT 60000 // timeout while trying to connect to mqtt , ms
#define EE_KEY 56       // rst key