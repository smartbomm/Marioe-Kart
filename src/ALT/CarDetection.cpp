#include "CarDetection.h"

// Outside of class
CarDetectionSensor *pointerToClass; // declare a pointer to CarDetectionSensor class

static void carDetectedISR_e(void) { // define global handler
  pointerToClass->carDetectedISR_i(); // calls class member handler
}


void CarDetectionSensor::begin(int controllerPin) {
    pin = controllerPin;
    pinMode(IR_SENSOR_PIN,INPUT);
    pointerToClass = this;
    attachInterrupt(digitalPinToInterrupt(IR_SENSOR_PIN),carDetectedISR_e,RISING);

};

void CarDetectionSensor::carDetectedISR_i() {
  if(!identified) {
    currentMicros = micros();                             // save current runtime
    identified = true;
  }
  if (pulseCnt<5) pulseCnt++;

  interval = currentMicros - previousMicros;            // interval to last interrupt call
  if ( interval > 30000 ){                              // minimal time gap between two cars
    identified = false;                                 // a new car has to be detected
    previousMicros = currentMicros;                     // save runtime for next interrupt call
    return;}                                            //
  if ( identified ) return;                             // if car already detected, skip interrupt
  if ( interval < 60 ) {                                // must be an error
    previousMicros = currentMicros;                     // save runtime for a new try
    return;}                                            //
  carID = (interval / 64) - 1;                          // at this point interval is valid, calculate car ID
  identified = true;                                    // indicate that we have already identified the car
  previousMicros = currentMicros;  
}

int CarDetectionSensor::getCarID() {
  return carID;
}
    
