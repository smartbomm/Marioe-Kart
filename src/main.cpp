#include <Arduino.h>
#include <Settings.h>
#include <CommunicationSPS/CommunicationSPS.h>
#include <CommunicationSPS/AnswerMC.h>


void setup() {
    #ifdef SERIAL_DEBUGGING
    Serial.begin(9600);
    #endif
    delay(2000);
    Serial.println("Test");
    comSPS_init();
    comSPS_sync();
}

void loop() {
    comSPS_execute();
}

void lifesignal(byte * buffer){
    comSPS_send2(0,1);
}

void comSPS_protocol(){
    comSPS_add(0, lifesignal);
}