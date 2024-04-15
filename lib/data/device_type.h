#pragma once

#include <Arduino.h>

enum DeviceType
{
    // DS
    bedroom_closet,
    bedroom_bed_lowerside,
    bedroom_bed_upperside,
    living_room_tv,
    living_room_piano,
    kitchen_workspace,
    office_monitor,
    office_desk,
    // NK
    kitchen_ceiling,
    // DEBUG
    debug,
};

struct DeviceConfig
{
    const char *localTopic;         // device topic
    uint16_t ledCount;              // strip led count
    uint8_t ledPin = D7;            // strip pin
    uint8_t btnPin = D5;            // button pin
    bool useBtn = 0;                // 1 - do use, 0 - do not
    uint16_t stripCurrent = 3000;   // led strip current limit
    const char *apName;
    const char *apPass = "550132550132";
};

constexpr DeviceConfig getConfig(DeviceType type) {
    switch(type) {
        // DS
        case bedroom_closet: return {.localTopic = "DSLY_Bedroom_Closet", .ledCount = 92, .apName = "Lightify Bedroom Closet"};
        case bedroom_bed_lowerside: return {.localTopic = "DSLY_Bedroom_Bed_Lowerside", .ledCount = 368, .stripCurrent = 6000, .apName = "Lightify Bedroom Bed lowerside"};
        case bedroom_bed_upperside: return {.localTopic = "DSLY_Bedroom_Bed_Upperside", .ledCount = 79, .apName = "DSLY_Bedroom_Bed_Upperside"};
        case living_room_tv: return {.localTopic = "DSLY_Livingroom_TV", .ledCount = 78, .useBtn = 1, .apName = "Lightify Livingroom TV"};
        case living_room_piano: return {.localTopic = "DSLY_Livingroom_Piano", .ledCount = 77, .apName = "Lightify Livingroom Piano"};
        case kitchen_workspace: return {.localTopic = "DSLY_Kitchen_Workspace", .ledCount = 156, .apName = "Lightify Kitchen Workspace"};
        case office_monitor: return {.localTopic = "DSLY_Office_Monitor", .ledCount = 95, .apName = "Lightify Office Monitor"};
        case office_desk: return {.localTopic = "DSLY_Office_Desk", .ledCount = 90, .apName = "Lightify Office Desk"};
        // NK
        case kitchen_ceiling: return {.localTopic = "DNLY_Kitchen_Ceiling", .ledCount = 141, .apName = "Lightify Kitchen Ceiling"};
        // DEBUG
        case debug: return {.localTopic = "DSLY_Debug_Lightify", .ledCount = 2, .apName = "Lightify Debug Device"};
    }
}