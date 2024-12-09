#include <Arduino.h>
#include <CarDetection.h>
#include <CommunicationSPS/CommunicationSPS.h>

void setup(){
    #ifdef SERIAL_DEBUGGING
    Serial.begin(9600);
    #endif

    carDect1_init(12);  //Car-Detection at Entry
    carDect2_init(13);  //Car-Detection at exit
    

    


}

uint32_t previous_time;

void loop() {
    uint32_t actualTime = micros();
    if(actualTime - previous_time>1000000) {
        Serial.println(carDect1_execute());
        previous_time = actualTime;
    }
    
    

}