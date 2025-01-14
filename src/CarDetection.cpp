#include <CarDetection.h>

/* Ablauf CarDetection:
 *                                 carDetected  |   detecting   |   Interrupt
 *                                 -------------|---------------|-------------------
 * 1. System ready for detection        false   |   false       |   active
 * 2. First pulse detected              false   |   true        |   active
 * 3. Second pulse detected             true    |   false       |   inactive
 * 4. execute(): calculate ID           false   |   false       |   inactive
 * 5. timeout runned out --> 1.         false   |   false       |   active
 */


//##########################################################
//CarDetection 1//
//##########################################################

uint8_t carDect1_pin;
volatile uint32_t carDect1_firstTime;
volatile uint32_t carDect1_lastTime;
volatile bool carDect1_detecting = false;
volatile bool carDect1_carDetected = false;

void carDect1_init (uint8_t pin) {  //initialize pin and variables
    carDect1_pin = pin;
    carDect1_firstTime = 0;
    carDect1_lastTime = 0;
    attachInterrupt(pin, carDect1_isr, FALLING);
}

/// [carDect1_exec]
uint8_t carDect1_execute(){    
    uint8_t car_id = 0;                                                      //Variable zur Speicherung der detektierten Reglernummer
    if(carDect1_carDetected){                                                //Fahrzeug wurde erkannt
        carDect1_carDetected = false;                                        //Flag zurücksetzen
        uint32_t period = carDect1_lastTime - carDect1_firstTime;            //Berechnung der Periodendauer zwischen den zwei Impulsen
        car_id =  (int)(period + IR_DECT_Period_Tolerance)/(64) - 1;         //Berechnung der Reglernummer unter Berücksichtigung der eingestellten Toleranz
        if(car_id>7 ) car_id = 99;                                           //Falls keine gültige Reglernummer berechnet werden konnte, wird 99 zurückgegeben
    } else car_id = 99;                                                      //Kein Fahrzeug erkannt  
    if(micros() - carDect1_firstTime > IR_DECT_CarTimeGap) attachInterrupt(carDect1_pin, carDect1_isr, FALLING);    //Wenn minimale Zeit zwischen zwei Fahrzeugen abgelaufen ist, wird der Interrupt wieder aktiviert
    return car_id;                                                        //Rückgabe der berechneten Reglernummer
}
/// [carDect1_exec]

/// [carDect1_isr]
void IRAM_ATTR carDect1_isr(){
    if(!carDect1_detecting){            //1. Impuls
        carDect1_firstTime =micros();   //Zeitstempel des ersten Impulses sichern
        carDect1_detecting = true;      //Flag setzen, dass erster Impuls erkannt wurde (Erkennung läuft)
    }
    else {                              //2. Impuls               
        carDect1_carDetected = true;    //Flag setzen, dass Fahrzeug erkannt wurde
        carDect1_detecting = false;     //Flag rücksetzen, dass Erkennung läuft
        carDect1_lastTime = micros();   //Zeitstempel des zweiten Impulses sichern
        detachInterrupt(carDect1_pin);  //Interrupt deaktivieren
    }
}
/// [carDect1_isr]

//##########################################################
//CarDetection 2//
//##########################################################

uint8_t carDect2_pin;

//-
volatile uint32_t carDect2_firstTime;
volatile uint32_t carDect2_lastTime;
volatile bool carDect2_detecting = false;
volatile bool carDect2_carDetected = false;

void carDect2_init (uint8_t pin) {  //initialize pin and variables
    carDect2_pin = pin;
    carDect2_firstTime = 0;
    carDect2_lastTime = 0;
    attachInterrupt(pin, carDect2_isr, FALLING);
}

uint8_t carDect2_execute(){    //to be executed in program loop, calculates car ids, if cars where detected
    uint8_t car_id = 0;
    if(carDect2_carDetected){
        carDect2_carDetected = false;
        uint32_t period = carDect2_lastTime - carDect2_firstTime;
        car_id =  (int)(period + IR_DECT_Period_Tolerance)/(64) - 1;        //calculate Car-ID under consideration of the setted tolerance
        if(car_id>7 ) car_id = 99;
    } else car_id = 99;
    if(micros() - carDect2_firstTime > IR_DECT_CarTimeGap) attachInterrupt(carDect2_pin, carDect2_isr, FALLING);
    return car_id;
}

void IRAM_ATTR carDect2_isr(){ 
    if(!carDect2_detecting){
        carDect2_firstTime =micros();
        carDect2_detecting = true;
    }
    else {
        carDect2_carDetected = true;
        carDect2_detecting = false;
        carDect2_lastTime = micros();
        detachInterrupt(carDect2_pin);
    }
}
