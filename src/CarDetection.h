#ifndef CARDETECTION_H
#define CARDETECTION_H
#include <Arduino.h>
#include "Definitions.h"

//Configuration of the CarDetectionUnit
//- initialize pin and variables
void carDect1_init (uint8_t pin);   
void carDect2_init (uint8_t pin); 

//to be executed in program loop, calculates car ids, if cars where detected and writes to buffer
//returns detected Car
//If no car was detected returns 99
uint8_t carDect1_execute();   
uint8_t carDect2_execute();

//Interrupt-Routine saves the period between two pulses for calculating the car-id
void IRAM_ATTR carDect1_isr();  
void IRAM_ATTR carDect2_isr();  





#endif 