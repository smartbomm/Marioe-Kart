#include <Arduino.h>
#include <Definitions.h>
#include <CommunicationSPS/CommunicationSPS.h>
#include <CarDetection.h>
#include <CarreraControll.h>



//carDect1  == CarDetection EntryLane
//carDect2  == CarDetection ExitLane

CarreraControll laneControl;
bool carOnPickingPlace = false;     //Is car standing on
uint8_t entryLaneQueue = 0;         // waiting car - id for putting in to storage
uint8_t lastProgrammedCar = 0;      




void setup() {
    pinMode(RELAY_EntryLane_p, OUTPUT);  //Relay Entry Lane
    pinMode(RELAY_ExitLane_p, OUTPUT);   //Relay Exit Lane
    laneControl.conf(DRIVER_ProgLane_p);     //Initialize Carrera Lane Protocol
    #ifdef SERIAL_DEBUGGING
    Serial.begin(9600);
    #endif
    delay(2000);

    carDect1_init(12);  //CarDetection Entry
    carDect2_init(13);  //CarDetection Exit

    comSPS_init();      //Initialize Serial Instance for Communication with SPS
    comSPS_sync();      //Sync µC with SPS

    digitalWrite(RELAY_EntryLane_p, HIGH);

}

void loop() {
    if(!carOnPickingPlace || entryLaneQueue > 0) {          //Car is waiting on programming lane for entry to storage
        laneControl.drive(entryLaneQueue, VEL_CarEntry);
        entryLaneQueue = false;
    }
    if(uint8_t carId = carDect1_execute() < 99) {           //Car is entering the programming lane
        comSPS_writeData(C_MC_CarIN(carId));
        if(!carOnPickingPlace) laneControl.drive(carId, VEL_CarEntry);
        else entryLaneQueue = carId;
    }
    if(uint8_t carId = carDect2_execute() < 99) comSPS_writeData(C_MC_CarOUT(carId));   //report to SPS: Car is exiting

    if(laneControl.program()) {
        comSPS_writeData(C_MC_CarPROGRAMMED(lastProgrammedCar));  //report to SPS: Car was programmed successfull
        digitalWrite(RELAY_EntryLane_p, HIGH);
    comSPS_execute();   //Execute Commands received from SPS
    }
    
}

//Answer to request from SPS
void request(byte * buffer){
    comSPS_sendDataPacket();
}
//Program id to car
void programCar(byte * buffer){
    digitalWrite(RELAY_EntryLane_p, LOW);
    digitalWrite(RELAY_ExitLane_p, HIGH);
    laneControl.program(buffer [C_SPS_PROGRAM_ID], buffer [C_SPS_PROGRAM_VMAX], buffer [C_SPS_PROGRAM_BRAKE], buffer [C_SPS_PROGRAM_FUEL]);
    comSPS_send2(C_MC_OK(buffer [C_SPS_PROGRAM_ID]));
    lastProgrammedCar = buffer [C_SPS_PROGRAM_ID];
    
}

//Drive car to exit the box
void driveCar(byte * buffer){
    comSPS_send2(C_MC_OK(buffer [C_SPS_PROGRAM_ID]));
    digitalWrite(RELAY_EntryLane_p, LOW);
    digitalWrite(RELAY_ExitLane_p, HIGH);
    laneControl.drive(buffer [1], VEL_CarExit);
    
}

void lightBarrier(byte * buffer){
    if(buffer[1] == 0xff) carOnPickingPlace = true;
    else if (buffer [1] == 0x00) {
        carOnPickingPlace = false;
        laneControl.driveAll(0);
    }


}


void comSPS_protocol(){
    comSPS_addCommand(0, request);
    comSPS_addCommand(1, programCar);
    comSPS_addCommand(5, driveCar);
    comSPS_addCommand(2, lightBarrier);
}