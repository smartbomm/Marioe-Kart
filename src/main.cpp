#include <Arduino.h>
#include <Definitions.h>
#include <CommunicationSPS/CommunicationSPS.h>
#include <CarDetection.h>




void setup() {
    #ifdef SERIAL_DEBUGGING
    Serial.begin(9600);
    #endif
    delay(2000);

    carDect1_init(12);  //CarDetection Entry
    carDect2_init(13);  //CarDetection Exit
    comSPS_init();      //Initialize Serial Instance for Communication with SPS
    comSPS_sync();      //Sync µC with SPS

}

void loop() {
    if(uint8_t carId = carDect1_execute() < 99) comSPS_writeData(C_MC_CarIN(carId));
    if(uint8_t carId = carDect2_execute() < 99) comSPS_writeData(C_MC_CarOUT(carId));
    comSPS_execute();
    
}

//Answer to request from SPS
void request(byte * buffer){
    comSPS_sendDataPacket();
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
}


void comSPS_protocol(){
    comSPS_addCommand(0, request);
    comSPS_addCommand(1, programCar);
    comSPS_addCommand(5, driveCar);
}