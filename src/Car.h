#ifndef Car_h
#define Car_h

#include <Arduino.h>
#include <Definitions.h>
#include <Settings.h>

struct car {
    uint8_t id = 99;
    struct car* next = NULL;
};


void addCar(uint8_t id);                //Lese Auto in Liste ein
uint8_t getCar();                       //Lese Auto aus Liste aus und lösche es


#endif