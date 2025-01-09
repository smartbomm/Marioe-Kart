#ifndef DebugLED_h
#define DebugLED_h

#include <Arduino.h>
#include <FastLED.h>

//Macht eigentlich nur mit RTOS Sinn

typedef struct {
    CRGB* led;
    CRGB color;
} debugLedAction_t;

void blinkOnce(void * parameter) {
    debugLedAction_t * action = (debugLedAction_t *) parameter;
    CRGB previousColor = *action->led;
    *action->led = action->color;
    taskYIELD();
    FastLED.show();
    vTaskDelay(500);
    *action->led = previousColor;
    vTaskDelete(NULL);
}
#endif