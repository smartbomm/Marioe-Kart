#include <Arduino.h>
#include <Definitions.h>
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

//Answer to Lifesignal from SPS
void lifesignal(byte * buffer){
    comSPS_send2(C_MC_LIFESIGNAL);
}
//Program id to car
void programCar(byte * buffer){
    comSPS_send2(C_MC_OK(buffer [C_SPS_PROGRAM_ID]));
    buffer [C_SPS_PROGRAM_ID];
    buffer [C_SPS_PROGRAM_VMAX];
    buffer [C_SPS_PROGRAM_BRAKE];
    buffer [C_SPS_PROGRAM_FUEL];
    
}

//Drive car to exit the box
void driveCar(byte * buffer){
    comSPS_send2(C_MC_OK(buffer [C_SPS_PROGRAM_ID]));
    DEBUG(Hallo Welt);
}


void comSPS_protocol(){
    comSPS_add(0, lifesignal);
    comSPS_add(1, programCar);
    comSPS_add(5, driveCar);
}