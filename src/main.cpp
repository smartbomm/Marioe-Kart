#include <Arduino.h>
#include <Definitions.h>
#include <CommunicationSPS/CommunicationSPS.h>
#include <CarDetection.h>
#include <CarreraControll.h>
#include <FastLED.h>
#include <DebugLED.h>
#include <OTA.h>


//carDect1  == CarDetection EntryLane
//carDect2  == CarDetection ExitLane

CarreraControll laneControl;
bool carOnPickingPlace = false;     //Is car standing on
uint8_t entryLaneQueue = 99;         // waiting car - id for putting in to storage
uint8_t lastProgrammedCar = 0;   
uint32_t SPS_lastLifeSignal = 0; 



CRGB led [1];

void programCar(byte * buffer);


void setup() {
    FastLED.addLeds<NEOPIXEL, RGB_LED>(led, 1);
    FastLED.setBrightness(DEBUG_RGB_BRIGHTNESS);
    led [0] = CRGB::Red;
    FastLED.show();

    #ifdef SERIAL_DEBUGGING
    Serial.begin(9600);
    delay(2000);
    #endif

    #ifdef OTA_Flashing
    setupOTA("CCU", OTA_SSID, OTA_PW);
    #endif

    led [0] = CRGB::Blue;
    FastLED.show();
    
    pinMode(RELAY_EntryLane_p, OUTPUT);  //Relay Entry Lane
    pinMode(RELAY_ExitLane_p, OUTPUT);   //Relay Exit Lane
    laneControl.conf(DRIVER_ProgLane_p);     //Initialize Carrera Lane Protocol
    

    carDect1_init(12);  //CarDetection Entry
    carDect2_init(13);  //CarDetection Exit

    #ifdef SPS_Connected
    comSPS_init();      //Initialize Serial Instance for Communication with SPS
    comSPS_sync();      //Sync µC with SPS
    SPS_lastLifeSignal = millis();
    #endif

    laneControl.driveAll(VEL_CarEntry);
    digitalWrite(RELAY_EntryLane_p, HIGH);
    led [0] = CRGB::Green;
    FastLED.show();
}

void loop() {

    //Debugging
    #ifdef SPS_Connected                //Checking if SPS is online
    uint32_t timestamp = millis();
    if(timestamp - SPS_lastLifeSignal > SPS_UART_Timeout*1000) {
        led [0] = CRGB::Red; FastLED.show();
        DEBUG(ALARM! SPS not reacheable!);
        comSPS_sync();
        SPS_lastLifeSignal = timestamp;
        led [0] = CRGB::Green; FastLED.show();
    }
    #endif
    if(!carOnPickingPlace && entryLaneQueue != 99) {          //Car is waiting on programming lane for entry to storage
        laneControl.drive(entryLaneQueue, VEL_CarEntry);
        entryLaneQueue = 99;
    }
    uint8_t carId;
    if((carId = carDect1_execute()) < 99) {           //Car is entering the programming lane
        DEBUGF("Car in - ID: %d\n", carId);
        comSPS_writeData(C_MC_CarIN(carId));
        if(!carOnPickingPlace) {
            laneControl.driveAll(0);
            laneControl.drive(carId, VEL_CarEntry_brake);
        } else {
            laneControl.driveAll(0);
            entryLaneQueue = carId;
        }
    }
    if((carId = carDect2_execute()) < 99) {           //report to SPS: Car is exiting
        DEBUGF("Car out - ID: %d\n", carId);
        comSPS_writeData(C_MC_CarOUT(carId));
        //laneControl.driveAll(0);
        digitalWrite(RELAY_ExitLane_p, LOW);
    }
           
    if(laneControl.program()) {
        comSPS_writeData(C_MC_CarPROGRAMMED(lastProgrammedCar));  //report to SPS: Car was programmed successfull
        digitalWrite(RELAY_EntryLane_p, HIGH);
        DEBUG(Programmieren fertig.);
    }
    comSPS_execute();   //Execute Commands received from SPS

}

//Answer to request from SPS
void request(byte * buffer){
    SPS_lastLifeSignal = millis();
    comSPS_sendDataPacket();
}
//Program id to car
void programCar(byte * buffer){
    comSPS_send2(C_MC_OK(buffer [C_SPS_PROGRAM_ID]));
    digitalWrite(RELAY_EntryLane_p, LOW);
    digitalWrite(RELAY_ExitLane_p, HIGH);
    laneControl.driveAll(0);
    laneControl.program(buffer [C_SPS_PROGRAM_ID], buffer [C_SPS_PROGRAM_VMAX], buffer [C_SPS_PROGRAM_BRAKE], buffer [C_SPS_PROGRAM_FUEL]);
    DEBUGF("Programming: ID: %d, V: %d, B: %d, F: %d\n", buffer [C_SPS_PROGRAM_ID], buffer [C_SPS_PROGRAM_VMAX], buffer [C_SPS_PROGRAM_BRAKE], buffer [C_SPS_PROGRAM_FUEL]);
    lastProgrammedCar = buffer [C_SPS_PROGRAM_ID];
    DEBUGF("EntryLaneQueue: %d \n", entryLaneQueue);
    
}

//Drive car to exit the box
void driveCar(byte * buffer){
    comSPS_send2(C_MC_OK(buffer [C_SPS_PROGRAM_ID]));
    digitalWrite(RELAY_EntryLane_p, LOW);
    digitalWrite(RELAY_ExitLane_p, HIGH);
    laneControl.drive(buffer [1], VEL_CarExit);
    DEBUGF("driveCar ID: %d, Vel: %d\n", buffer [1], VEL_CarExit);
    delay(2000);
    digitalWrite(RELAY_ExitLane_p, LOW);
    if (!carOnPickingPlace) {
        digitalWrite(RELAY_EntryLane_p, HIGH);
        laneControl.driveAll(VEL_CarEntry);
    }
}

void lightBarrier(byte * buffer){
    if(buffer[1] == 0xff) {
        comSPS_send2(C_MC_OK(0xff));
        carOnPickingPlace = true;
        laneControl.driveAll(0);
        DEBUG(Light Barrier: Car is on picking place.)
    }
    else if (buffer [1] == 0x00) {
        comSPS_send2(C_MC_OK(0xfe));
        carOnPickingPlace = false;
        digitalWrite(RELAY_EntryLane_p, HIGH);
        digitalWrite(RELAY_ExitLane_p, LOW);
        laneControl.driveAll(VEL_CarEntry);
        DEBUG(Light Barrier: Picking Place is free.)
    }
}

void mcReady (byte * buffer){
    comSPS_send2(C_MC_OK(6));
}

void comSPS_protocol(){
    comSPS_addCommand(0, request);
    comSPS_addCommand(1, programCar);
    comSPS_addCommand(5, driveCar);
    comSPS_addCommand(2, lightBarrier);
    comSPS_addCommand(6, mcReady);
}