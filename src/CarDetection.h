#ifndef CARDETECTION_H
#define CARDETECTION_H
#include <Arduino.h>
#include "Definitions.h"

//Configuration of the CarDetectionUnit
//- initialize pin and variables
void carDect1_init (uint8_t pin);   

//to be executed in program loop, calculates car ids, if cars where detected and writes to buffer
void carDect1_execute();   

//returns detected cars from buffer, if no new car is available returns 99
uint8_t carDect1_getCarId();   

//Interrupt-Routine Count to 5 pulses, save the timestamps and then stop the interrupt routine until minimal timegap between two cars has run out
void IRAM_ATTR carDect1_isr();  

uint8_t carDect1_pin;
//CarDection LIFO-Buffer
uint8_t carDect1_detectionBuffer [IR_DECT_BufferSize]= {99};
uint8_t carDect1_nextRead;
uint8_t carDect1_nextWrite;
//-
volatile uint8_t carDect1_count;
volatile uint32_t carDect1_firstTime;
volatile uint32_t carDect1_lastTime;



#endif 