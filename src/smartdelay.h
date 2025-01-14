#ifndef SMARTDELAY_H
#define SMARTDELAY_H

#include <Arduino.h>

typedef struct {
    uint32_t timeActivated = 0;
    bool active = false;
    uint32_t delayTime_ms = 0;
} smartdelay_t;

void smartdelay(smartdelay_t *sd) {
    if (sd->delayTime_ms > 0) 
    {
        sd->timeActivated = millis();
        sd->active = true;
    }
}

bool smartdelay_check(smartdelay_t *sd) 
{
    if (sd->active && (millis() - sd->timeActivated) >= sd->delayTime_ms) 
    {
        sd->active = false;
        sd->timeActivated = 0;
        return true;
    }
    return false;
}

#endif // SMARTDELAY_H