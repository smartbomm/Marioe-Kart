#ifndef CARDETECTION_H
#define CARDETECTION_H
#include <Arduino.h>
#include "Definitions.h"

class CarDetectionSensor {
    private:
        int pin;
        volatile unsigned int carDetectedDelay;
        volatile unsigned long carDetectedTime;
        
    public:
        void begin(int controllerPin);
        void carDetectedISR_i();
};

#endif