#ifndef DebugLED_h
#define DebugLED_h

#include <Arduino.h>
#include <FastLED.h>

//Macht eigentlich nur mit RTOS Sinn

enum debugLed_mode {
    STATE_ON, 
    BLINKING,
    BLINK_ONCE
};

typedef struct {
    CRGB * led;
    CRGB color;
    debugLed_mode mode;
    uint16_t t_blink_on_us;
    uint16_t t_blink_off_us;
    uint32_t timestamp;
    bool state;
    CRGB previousColor;
} debugLed_action_t;

void debugLed(debugLed_action_t * action) {
    action->previousColor = *action->led;
    switch (action->mode) {
        case STATE_ON:
            *(action->led) = action->color;
            FastLED.show();
        case BLINKING:

        case BLINK_ONCE:
        *(action->led) = action->color;


    } 
}

#endif