

/**
 * @file main.cpp
 * @authors Stefan Graml (stefan.graml@hof-university.de), 
 * 
 *  Joel Bommeli (joel.bommeli@hof-university.de)
 * @brief Hauptprogramm der CarControlUnit
 * 
 * Bezeichnung | Ausführung
 * ------------|----------
 * Mikrocontroller | ESP-32-S3
 * Programmiersprache | C++
 * Framework | Arduino
 * 
 * carDect1  == CarDetection Einfahrtsschiene
 * carDect2  == CarDetection Ausfahrtsschiene
 * 
 * @version 0.1
 * @date 2024-12-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//##########################################################################################//

//Includes
#include <Arduino.h>                //Arduino Framework
#include <Definitions.h>            //Hardwarespezifische Definitionen
#include <CommunicationSPS.h>       //Kommunikation mit SPS
#include <CarDetection.h>           //Fahrzeugerkennung
#include <CarreraControll.h>        //Bahnsteuerung
#include <FastLED.h>                //Ansteuerung der Debug-RGB-LED
#include <OTA.h>                    //Over-the-Air-Flashing, Telnet-Debugging


//Funktionsdeklarationen
void request(byte * buffer);
void programCar(byte * buffer);
void driveCar(byte * buffer);
void lightBarrier(byte * buffer);

///@name Variablen
///@{
bool carOnPickingPlace = false;     ///<Fahrzeug auf Greifplatz
uint8_t lastProgrammedCar = 0;      ///<Zuletzt programmiertes Fahrzeug
uint32_t SPS_lastLifeSignal = 0;    ///<Zeitpunkt des letzten von der SPS empfangenen Polling-/Syncsignals
///@}

///@name Objekte
///@{
CarreraControll laneControl;        ///<Bahnsteuerung
CRGB led [1];                       ///<Config-Strukur für Debug-RGB-LED
///@}


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
    
    carDect1_init(CAR_Detect_EntryLane_p);  //CarDetection Entry
    carDect2_init(CAR_Detect_ExitLane_p);  //CarDetection Exit

    #ifdef SPS_Connected

    ///@brief Definition der Befehle, die von der SPS empfangen werden können
    comSPS_addCommand(0, request);  
    comSPS_addCommand(1, programCar);
    comSPS_addCommand(2, lightBarrier);
    comSPS_addCommand(5, driveCar);

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

/**
 * @brief Auszuführende Funktion für Befehlspaket "Polling-/Syncsignal" von der SPS
 * 
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
 * 
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
 * 
 * Schaltet die Einfahrtsschiene aus und die Ausfahrtsschiene ein und lässt anschließend das Fahrzeug mit der übermittelten Reglernummer ausfahren
 * Nach einer 
 * 
 * @param buffer Datenpaket das von der SPS empfangen wurde
 */
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

/**
 * @brief Auszuführende Funktion für das Befehlspaket "Lichtschranke" von der SPS
 * 
 * Schaltet die Einfahrtsschiene aus, wenn ein Fahrzeug auf dem Greiferplatz steht. Wenn der Greiferplatz frei ist, wird die Einfahrtsschiene eingeschaltet und die Ausfahrtsschiene ausgeschaltet.
 * 
 * @param buffer Datenpaket das von der SPS empfangen wurde
 */
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
