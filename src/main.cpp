#include <Arduino.h>
#include <Definitions.h>
#include <CommunicationSPS/CommunicationSPS.h>
#include <CarDetection.h>
#include <CarreraControll.h>
#include <FastLED.h>


//carDect1  == CarDetection EntryLane
//carDect2  == CarDetection ExitLane

CarreraControll laneControl;
bool carOnPickingPlace = false;     //Is car standing on
uint8_t entryLaneQueue = 0;         // waiting car - id for putting in to storage
uint8_t lastProgrammedCar = 0;   
uint32_t SPS_lastLifeSignal = 0;   

CRGB led [1];




void setup() {
    FastLED.addLeds<NEOPIXEL, RGB_LED>(led, 1);
    FastLED.setBrightness(DEBUG_RGB_BRIGHTNESS);
    led [0] = CRGB::Red;
    FastLED.show();

    #ifdef SERIAL_DEBUGGING
    Serial.begin(9600);
    delay(2000);
    #endif
    
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

    led [0] = CRGB::Green;
    FastLED.show();
    



}

void loop() {
    #ifdef SPS_Connected
    uint32_t timestamp = millis();
    if(timestamp - SPS_lastLifeSignal > SPS_UART_Timeout*1000) {
        led [0] = CRGB::Red; FastLED.show();
        DEBUG(ALARM! SPS not reacheable!);
        bool syncSuccessfull = false;
        while(syncSuccessfull == false) {
            comSPS_sync();
            syncSuccessfull = true;
        }
        SPS_lastLifeSignal = timestamp;
        led [0] = CRGB::Green; FastLED.show();
    }
    #endif
    if(!carOnPickingPlace || entryLaneQueue > 0) {          //Car is waiting on programming lane for entry to storage
        laneControl.drive(entryLaneQueue, VEL_CarEntry);
        entryLaneQueue = false;
    }
    if(uint8_t carId = carDect1_execute() < 99) {           //Car is entering the programming lane
        DEBUGF("Car in - ID: %d\n", carId);
        led [0] = CRGB::Blue;
        FastLED.show();
        led [0] = CRGB::Green;
        FastLED.show();
        comSPS_writeData(C_MC_CarIN(carId));
        if(!carOnPickingPlace) {
            digitalWrite(RELAY_EntryLane_p, HIGH);
            laneControl.drive(carId, VEL_CarEntry);
        } else {
            entryLaneQueue = carId;
        }
    }
    if(uint8_t carId = carDect2_execute() < 99) {           //report to SPS: Car is exiting
        DEBUGF("Car out - ID: %d\n", carId);
        led [0] = CRGB::Blue;
        FastLED.show();
        led [0] = CRGB::Green;
        FastLED.show();
        comSPS_writeData(C_MC_CarOUT(carId));
    }
           
    if(laneControl.program()) {
        comSPS_writeData(C_MC_CarPROGRAMMED(lastProgrammedCar));  //report to SPS: Car was programmed successfull
        digitalWrite(RELAY_EntryLane_p, HIGH);
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