#include <Arduino.h>
#include <Definitions.h>
#include <CommunicationSPS/CommunicationSPS.h>
#include <CommunicationSPS/AnswerMC.h>
#include <CarDetection.h>




void setup() {
    #ifdef SERIAL_DEBUGGING
    Serial.begin(9600);
    #endif
    delay(2000);
    Serial.println("Test");
    carDect1_init(12);
    comSPS_init();
    comSPS_sync();

}

void loop() {
    comSPS_execute();
    carDect1_execute();
    if(uint8_t carId = carDect1_getCarId() < 99) comSPS_writeData(1, carId);
}

//Answer to request from SPS
void request(byte * buffer){
    comSPS_sendData();
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
    comSPS_addCommand(0, request);
    comSPS_addCommand(1, programCar);
    comSPS_addCommand(5, driveCar);
}