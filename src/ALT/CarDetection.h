#ifndef CARDETECTION_H
#define CARDETECTION_H
#include <Arduino.h>
#include "Definitions.h"

class CarDetectionSensor {
    private:
        int pin;
        bool detectionActive;
        volatile unsigned int microsDelay;
        volatile unsigned long lastMicros;

        int carID = 99;                                         // save current car ID
        
        unsigned long previousMicros = 0;                       // last runtime of interrupt call
        unsigned long currentMicros = 0;                        // current runtime 
        unsigned long interval = 0;                             // time interval between last and current interrupt call

        int pulseCnt;
        boolean identified = false;                             // indicates that car has already been indentified
        
        
    public:
        void begin(int controllerPin);
        void carDetectedISR_i();
        int getCarID();
};

#endif 