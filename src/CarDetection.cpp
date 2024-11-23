#include <CarDetection.h>

void carDect1_init (uint8_t pin) {  //initialize pin and variables
    carDect1_pin = pin;
    carDect1_firstTime = 0;
    carDect1_lastTime = 0;
    carDect1_count = 0;
}

uint8_t carDect1_execute(){    //to be executed in program loop, calculates car ids, if cars where detected and writes to buffer
    uint8_t car_id;
    if(carDect1_count==5){
        uint32_t interval = carDect1_lastTime - carDect1_firstTime;
        car_id =  (int)(interval + 5*IR_DECT_Period_Tolerance)/320;        //calculate Car-ID under consideration of the setted tolerance
        carDect1_count = 0;
    } else car_id = 99;
    if(micros()-carDect1_firstTime>=IR_DECT_CarTimeGap) attachInterrupt(digitalPinToInterrupt(carDect1_pin), carDect1_isr, FALLING);
    return car_id;
}

void IRAM_ATTR carDect1_isr(){                  //Count to 5 pulses, save the timestamps and then stop the interrupt routine until minimal timegap between two cars has run out
    detachInterrupt(carDect1_pin);
    uint32_t currentTime = micros();
    if (currentTime - carDect1_lastTime>=(IR_DECT_Period_min - IR_DECT_Period_Tolerance)){
        if(carDect1_count == 0) carDect1_firstTime = currentTime;
        carDect1_count++;
        carDect1_lastTime = currentTime;
    }
    if(carDect1_count < 5) {
        attachInterrupt(digitalPinToInterrupt(carDect1_pin), carDect1_isr, FALLING);
    }
}