#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <Arduino.h>
#include <CommunicationSPS/CommunicationSPS.h>

void comSPS_protocol();
void exampleCommand(byte * buffer);
void anotherExampleCommand(byte * buffer);


void setup() {

    comSPS_init();      //Initialisiert die serielle Kommunikation mit der SPS
    comSPS_sync();      //Synchronisiert die serielle Kommunikation mit der SPS


}

void loop() {
    comSPS_execute();   //Funktion zum Ausführen von Befehlen, die von der SPS empfangen wurden


}

void comSPS_protocol(){
    comSPS_addCommand(0, exampleCommand);           //Füge die Funktion exampleCommand hinzu, die ausgeführt wird, wenn der Befehl 0 empfangen wird
    comSPS_addCommand(1, anotherExampleCommand);    //Füge die Funktion anotherExampleCommand hinzu, die ausgeführt wird, wenn der Befehl 1 empfangen wird
}

void exampleCommand(byte * buffer){
    //Führe Befehl 0 aus

    //Aufruf von Daten aus dem empfangenen Befehlspaket
    uint8_t data = buffer[1];

}

#endif // EXAMPLE_H