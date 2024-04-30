#include "time_setup.h"

void setupTime() {
  ntp.setUpdateInterval(NTP_UPD_PRD * 60000ul / 2); 
  ntp.setTimeOffset((cfg.GMT) * 3600l);
  ntp.setPoolServerName(NTPserver);
  DEBUG(F("NTP Setup. Time zone: "));
  DEBUGLN(cfg.GMT);
  ntp.begin();
  if (ntp.update()) isTimeReady = true;
}