#include <CarDetection.h>

void carDect1_init (uint8_t pin) {  //initialize pin and variables
    carDect1_pin = pin;
    carDect1_firstTime = 0;
    carDect1_lastTime = 0;
    carDect1_nextRead = 0;
    carDect1_nextWrite = 0;
    carDect1_count = 0;
}

void carDect1_execute(){    //to be executed in program loop, calculates car ids, if cars where detected and writes to buffer
    if(carDect1_count==5){
        uint32_t interval = carDect1_lastTime - carDect1_firstTime;
        carDect1_detectionBuffer[carDect1_nextWrite] = (int)(interval + 5*IR_DECT_Period_Tolerance)/320;        //calculate Car-ID under consideration of the setted tolerance
        carDect1_count = 0;
        if(carDect1_nextWrite < IR_DECT_BufferSize-1) carDect1_nextWrite++; //Move pointer to next value in FIFO-Buffer
        else carDect1_nextWrite = 0;
    }
    if(micros()-carDect1_firstTime>=IR_DECT_CarTimeGap) attachInterrupt(digitalPinToInterrupt(carDect1_pin), carDect1_isr, FALLING);
}

uint8_t carDect1_getCarId() {                       //returns detected cars from buffer, if no new car is available returns 99
    if(carDect1_nextRead != carDect1_nextWrite) {
        uint8_t carId = carDect1_detectionBuffer[carDect1_nextRead];
        if(carDect1_nextRead < IR_DECT_BufferSize-1) carDect1_nextRead++;   //Move pointer to next value in FIFO-Buffer
        else carDect1_nextRead = 0;
        return carId;
    } else return 99;
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