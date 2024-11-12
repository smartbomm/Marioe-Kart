#include <Arduino.h>
#include <Settings.h>
#include <CommunicationSPS/CommunicationSPS.h>
#include <CommunicationSPS/AnswerMC.h>


void setup() {
    comSPS_init();
    comSPS_sync();
}

void loop() {
    comSPS_execute();
}
void lifesignal(byte * buffer){
    comSPS_send2(0,0);
}

void comSPS_protocol(){
    comSPS_add(48, lifesignal);
}

