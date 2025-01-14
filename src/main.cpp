

/**
 * @file main.cpp
 * @author Joel Bommeli (joel.bommeli@hof-university.de)
 * @brief Hauptprogramm der CarControlUnit
 * 
 * Mikrocontroller | ESP-32-S3
 * Programmiersprache | C++
 * Framework | Arduino
 * 
 * @version 0.1
 * @date 2024-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Arduino.h>
#include <Definitions.h>
#include <CommunicationSPS.h>
#include <CarDetection.h>
#include <CarreraControll.h>
#include <FastLED.h>
#include <OTA.h>
#include <smartdelay.h>


//carDect1  == CarDetection EntryLane
//carDect2  == CarDetection ExitLane


//Functions

void request(byte * buffer);
void programCar(byte * buffer);
void driveCar(byte * buffer);
void lightBarrier(byte * buffer);
void mcReady (byte * buffer);
void comSPS_protocol();


//Variables

bool carOnPickingPlace = false;     //Is car standing on
uint8_t entryLaneQueue = 99;         // waiting car - id for putting in to storage
uint8_t lastProgrammedCar = 0;   
uint32_t SPS_lastLifeSignal = 0; 


//Objects

smartdelay_t drivingOutDelay;    ///<Zeit für die die Ausfahrschiene zur Ausfahrt eines Fahrzeugs aktiviert sein muss, bis sie wieder sugeschaltet werden kann
CarreraControll laneControl;    ///<Ansteuerung des Bahntreibers
CRGB led [1];                       ///<Config-Objekt für Debugging-RGB-LED auf MIkrocontroller-Board


/**
 * @brief Arduino Setup-Funktion
 * Wird einmalig beim Programmstart ausgeführt
 * 
 */
void setup() {
    drivingOutDelay.delayTime_ms = 2000;
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
    
    carDect1_init(CAR_Detect_EntryLane_p);  //CarDetection Entry
    carDect2_init(CAR_Detect_ExitLane_p);  //CarDetection Exit

    #ifdef SPS_Connected

    ///@brief Definition der Befehle, die von der SPS empfangen werden können
    comSPS_addCommand(0, request);  
    comSPS_addCommand(1, programCar);
    comSPS_addCommand(2, lightBarrier);
    comSPS_addCommand(5, driveCar);
    comSPS_addCommand(6, mcReady);

    comSPS_init();      //Initialize Serial Instance for Communication with SPS
    comSPS_sync();      //Sync µC with SPS
    SPS_lastLifeSignal = millis();
    #endif

    laneControl.driveAll(VEL_CarEntry);
    digitalWrite(RELAY_EntryLane_p, HIGH);
    led [0] = CRGB::Green;
    FastLED.show();
}

/**
 * @brief Arduino-Loop-Funktion
 * Wird nach der Setup-Funktion in Endlosschleife ausgeführt
 * 
 */
void loop() {
    #ifdef SPS_Connected                //Checking if PLC is online
    uint32_t timestamp = millis();
    if(timestamp - SPS_lastLifeSignal > SPS_UART_Timeout*1000) {
        led [0] = CRGB::Red; FastLED.show();
        DEBUG(ALARM! SPS not reacheable!);
        comSPS_sync();
        SPS_lastLifeSignal = timestamp;
        led [0] = CRGB::Green; FastLED.show();
    }
    #endif

    if (smartdelay_check(&drivingOutDelay))
    {
        digitalWrite(RELAY_ExitLane_p, LOW);
        if (!carOnPickingPlace)
        {
            digitalWrite(RELAY_EntryLane_p, HIGH);
            laneControl.driveAll(VEL_CarEntry);
        }
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

/**
 * @brief Auszuführende Funktion für Befehlspaket "Polling-/Syncsignal" von der SPS
 * Antwortet auf das Life-/Polling-Signal der SPS. 
 * Wenn zu übremittelnde Daten im Puffer vorhanden sind, werden diese gesendet, ansonsten wird mit dem Life-Siganl geantwortet.
 * 
 * @param buffer Datenpaket das von der SPS empfangen wurde
 */
void request(byte * buffer){
    SPS_lastLifeSignal = millis();
    comSPS_sendDataPacket();
}

/**
 * @brief Auszuführende Funktion für das Befehlspaket "Fahrzeug programmieren" von der SPS
 * Programmieren eines Fahrzeugs auf eine ID, Maximalgeschwindigkeit, Bremskraft und Treibstoffverhalten
 * 
 * @param buffer Datenpaket das von der SPS empfangen wurde
 */
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

/**
 * @brief Auszuführende Funktion für das Befehlspaket "Fahrzeug ausfahren" von der SPS
 * Schaltet die Einfahrtsschiene aus und die Ausfahrtsschiene ein und lässt anschließend das Fahrzeug mit der übermittelten Reglernummer ausfahren
 * Nach einer 
 * 
 * @param buffer 
 */
void driveCar(byte * buffer){
    comSPS_send2(C_MC_OK(buffer [C_SPS_PROGRAM_ID]));
    digitalWrite(RELAY_EntryLane_p, LOW);
    digitalWrite(RELAY_ExitLane_p, HIGH);
    laneControl.drive(buffer [1], VEL_CarExit);
    DEBUGF("driveCar ID: %d, Vel: %d\n", buffer [1], VEL_CarExit);
    smartdelay(&drivingOutDelay);
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