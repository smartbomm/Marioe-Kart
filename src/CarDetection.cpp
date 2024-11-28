#include <CarDetection.h>


uint8_t carDect1_pin;

//-
volatile uint32_t carDect1_firstTime;
volatile uint32_t carDect1_lastTime;

/* Ablauf CarDetection:
 *                                      enable  |   detecting
 *                                     ---------|-------------
 * 1. System bereit zur Erkennung       true    |   false
 * 2. Erster Impuls erkannt             false   |   true
 * 3. Zweiter Impuls erkannt            false   |   false
 * 4. ID berechnet                      false   |   false
 * 5. Timeout abgelaufen --> 1.         true    |   false
 */
volatile bool carDect1_detecting = false;
volatile bool carDect1_carDetected = false;

void carDect1_init (uint8_t pin) {  //initialize pin and variables
    carDect1_pin = pin;
    carDect1_firstTime = 0;
    carDect1_lastTime = 0;
    attachInterrupt(pin, carDect1_isr, FALLING);
}

uint8_t carDect1_execute(){    //to be executed in program loop, calculates car ids, if cars where detected and writes to buffer
    uint8_t car_id = 0;
    if(carDect1_carDetected){
        carDect1_carDetected = false;
        uint32_t period = carDect1_lastTime - carDect1_firstTime;
        DEBUGF("Periode = %d\n", period);
        car_id =  (int)(period + IR_DECT_Period_Tolerance)/(64);        //calculate Car-ID under consideration of the setted tolerance
        if(car_id>8 || car_id == 0) car_id = 99;
    } else car_id = 99;
    if(micros() - carDect1_firstTime > IR_DECT_CarTimeGap) attachInterrupt(carDect1_pin, carDect1_isr, FALLING);
    return car_id;
}

void IRAM_ATTR carDect1_isr(){                  //Count to 5 pulses, save the timestamps and then stop the interrupt routine until minimal timegap between two cars has run out
    if(!carDect1_detecting){
        carDect1_firstTime =micros();
        carDect1_detecting = true;
    }
    else {
        carDect1_carDetected = true;
        carDect1_detecting = false;
        carDect1_lastTime = micros();
        detachInterrupt(carDect1_pin);
    }
}