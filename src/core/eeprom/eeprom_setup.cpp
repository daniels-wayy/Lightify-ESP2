#include "eeprom_setup.h"

void EE_setup() {
    EEPROM.begin(1000);
    delay(100);
    if (EEPROM.read(0) != EE_KEY) {
        EEPROM.write(0, EE_KEY);
        EEPROM.put(1, cfg);
        EEPROM.put(sizeof(cfg) + 1, workflows);
        EEPROM.put(sizeof(cfg) + sizeof(workflows) + 1, modes);
        EEPROM.commit();
    }

    EEPROM.get(1, cfg);
    EEPROM.get(sizeof(cfg) + 1, workflows);
    EEPROM.get(sizeof(cfg) + sizeof(workflows) + 1, modes);

    if (cfg.ledCount > MAX_LEDS) {
        cfg.ledCount = MAX_LEDS;
    }

    DEBUGLN("EEPROM setup completed!");
}

void EE_erase() {
    EEPROM.begin(1000);
    delay(100);
    
    EEPROM.write(0, 0);
    EEPROM.commit();

    DEBUGLN("EEPROM erase completed!");
}