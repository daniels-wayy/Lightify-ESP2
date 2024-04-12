#include "time_ticker.h"

void timeTicker()
{
    static Timer tmr(500);
    if (tmr.period())
    {
        now.sec = ntp.getSeconds();
        now.min = ntp.getMinutes();
        now.hour = ntp.getHours();
        now.day = ntp.getDay();
        now.weekMs = now.getWeekS() * 1000ul + ntp.getMillis();
        now.setMs(ntp.getMillis());
        if (now.sec == 0 && now.min % NTP_UPD_PRD == 0 && ntp.update())
            isTimeReady = true;
    }
}