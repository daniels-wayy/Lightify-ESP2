#pragma once

#include <Arduino.h>

struct Timer {
  Timer () {
    state = 0;
  }
  Timer (uint32_t nprd, bool nstate = true) {
    state = nstate;
    setPeriod(nprd);
    tmr = millis();
  }

  bool isRunning() {
    return state;
  }

  void setPeriod(uint32_t nprd) {
    prd = nprd;
  }

  void restart() {
    tmr = millis();
    state = true;
  }

  bool elapsed() {
    return (!state || check());
  }

  bool period() {
    if (state && check()) {
      restart();
      return 1;
    } return 0;
  }

  bool check() {
    return millis() - tmr >= prd;
  }

  byte getLength8() {
    return (state) ? ((min(uint32_t(millis() - tmr), prd)) * 255ul / prd) : 0;
  }

  void stop() {
    state = 0;
  }

  uint32_t tmr, prd;
  bool state = 1;
};