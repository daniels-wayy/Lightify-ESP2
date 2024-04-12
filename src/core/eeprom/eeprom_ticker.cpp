#include "eeprom_ticker.h"

void EE_ticker() {
    if (EETimer.period()) {
        if (EECfgFlag || EEWorkflowsFlag) {
            if (EECfgFlag) {
                EECfgFlag = false;
                EEPROM.put(1, cfg);
                DEBUG("EEPROM cfg update occured! Size: ");
                DEBUGLN(sizeof(cfg));
            }
            if (EEWorkflowsFlag) {
                EEWorkflowsFlag = false;
                EEPROM.put(sizeof(cfg) + 1, workflows);
                DEBUG("EEPROM workflows update occured! Size: ");
                DEBUGLN(sizeof(workflows));
            }
            if (EEFxFlag) {
                EEFxFlag = false;
                EEPROM.put(sizeof(cfg) + sizeof(workflows) + 1, modes);
                DEBUG("EEPROM effects update occured! Size: ");
                DEBUGLN(sizeof(modes));
            }
            EEPROM.commit();
        }
        EETimer.stop();
    }
}

// cfg
void EE_updateCfg() {
    EECfgFlag = true;
    EETimer.restart();
}
void EE_updateCfgNow() {
    EEPROM.put(1, cfg);
    EEPROM.commit();
}
void EE_updateCfgRst() {
    EE_updateCfgNow();
    delay(100);
    ESP.restart();
}

// workflows
void EE_updateWorkflows() {
    EEWorkflowsFlag = true;
    EETimer.restart();
}
void EE_updateWorkflowsNow() {
    EEPROM.put(sizeof(cfg) + 1, workflows);
    EEPROM.commit();
}

// effects
void EE_updateEffects() {
    EEFxFlag = true;
    EETimer.restart();
}
void EE_updateEffectsNow() {
    EEPROM.put(sizeof(cfg) + sizeof(workflows) + 1, modes);
    EEPROM.commit();
}