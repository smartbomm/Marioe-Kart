#include "CarDetection.h"

// Outside of class
CarDetectionSensor *pointerToClass; // declare a pointer to CarDetectionSensor class

static void carDetectedISR_e(void) { // define global handler
  pointerToClass->carDetectedISR_i(); // calls class member handler
}


void CarDetectionSensor::begin(int controllerPin) {
    pin = controllerPin;
    pinMode(IR_SENSOR_PIN,INPUT_PULLDOWN);
    pointerToClass = this;
    attachInterrupt(digitalPinToInterrupt(IR_SENSOR_PIN),carDetectedISR_e,RISING);

};

void CarDetectionSensor::carDetectedISR_i() {
    volatile unsigned long microS = micros();
    carDetectedDelay = microS - carDetectedTime;
    carDetectedTime = microS;
}
    
