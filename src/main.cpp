#include <Arduino.h>
#include <CarDetection.h>

void setup(){
    carDect1_init(12);
    Serial.begin(9600);


}

uint32_t previous_time;

void loop() {
    uint32_t actualTime = micros();
    if(actualTime - previous_time>1000000) {
        Serial.println(carDect1_execute());
        previous_time = actualTime;
    }
    
    

}