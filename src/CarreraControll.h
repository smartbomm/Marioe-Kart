#ifndef CARRERACONTROLL_H
#define CARRERACONTROLL_H

/**
 * @page Fahrzeugsteuerung
 * @section Init Initalisierung
 * @snippet ./CarreraControll.cpp CarreraControllInit
 * zuerst wird ein getimter Interrupt erstellt dieser ruft regelmäßig die Funktion cycle() auf. 
 * Diese führt dann die nächste Aktion aus Diesen Teil würde ich nicht ändern solange das Protokoll laufen soll wie geplant. 
 * Falls es Änderungen am Timing geben soll kann man das in der initTime() Funktion ändern.
 * (ACHTUNG: einige pausenzeiten zählen zusätzlich hoch also werden alle timings gestreckt wenn man hier etwas ändert)
 * @snippet ./CarreraControll.cpp CarreraControllVariables
 * Hier werden die Variablen für die CarreraControll Klasse definiert.
 * @warning Achtung hier gibt es einige Flaggs die villeicht 
 * 
 * @snippet ./CarreraControll.cpp CarreraControllCycle
 */




/**
 * @file CarreraControll.h
 * @author Stefan Graml (stefan.graml@hof-university.de)
 * @brief The header of CarreraControll
 * @version 0.1
 * @date 2025-01-08
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Arduino.h>
/**
 * @brief Die Klasse CarreraControll übernimmt alle ansteuerungs Funktionen der Carrera Autos
 * Die Klasse wird momentan nur einmal verwendet. Die Klasse wurde am Anfang des Projektes erstellt, 
 * allerdings könnte sie überarbeitet werden, um mehrere Treiber auf einer CarControllUnit zu betreiben.
 * @note die Klasse ist nur einmal erstellt und nicht weiter getestet (außer Einzelbetrieb)
 */
class CarreraControll {
public:

    CarreraControll();
    static void IRAM_ATTR timerInterrupt();


    /**
     * @brief Initialisieren der CarreraControll Bibliothek
     * 
     * @param pin Ausgabepin der CarreraControll Bibliothek
     */
    void conf(int pin);



    /**
     * @brief Setzt die ID eines Fahrzeugs auf mitgegebene ID
     * 
     * @param carID Die ID die Auf das mommentane Fahrzeug gesetzt werden soll
     */
    void setID(int carID);


    /**
     * @brief Frage ab ob das Programieren einer ID erfolgreich durch gelaufen ist
     * @details Diese Funktion wird genau wie die Rückmeldung des Programmierens nur einmalig zurückgegeben
     */
    bool setID();


        /**
     * @brief Setzten der Geschwindigeit eines Carrera-Autos auf eine Geschwindigeit
     * 
     * @param carNr Die ID des zu programmierenden Autos
     * @param Speed Die zu setzende Geschwindigeit
     */
    void drive(int CarNr, int Speed);

    /**
     * @brief Programmieren des Autos auf eine ID, Maximalgeschwindigeit, Bremskraft und Tankgröße
     * 
     * @param carNr Die zu programmierende ID
     * @param maxSpeed Die zu programmierende Maximalgeschwindigeit
     * @param brake Die zu programmierende Bremskraft
     * @param fuel Die zu programmierende Tankgröße
     * 
     * @note Die Funktion wurde nur mit den validen werten 0-5 getestet.
     */
    void program(int carNr, int maxSpeed, int brake, int fuel);

    /**
     * @brief Gibt den programmiert Status einmalig zurück
     * 
     * @return true Das Auto wurde programmiert (setzt sich nach abrufen selbst zurück)
     * 
     * @return false Zustand bei nicht programmiertem Auto
     */
    bool program();

    /**
     * @brief Setzt die Geschwindigeit aller Carrera-Autos auf eine Geschwindigeit
     * 
     * @param value Der Wert der Geschwindigeit die alle Autos annehmen sollen
     */
    void driveAll(int value);
    /**
     * @brief Zyklus-Klasse (sollte nicht von außen aufgerufen werden muss aber public bleiben wegen den Interrupts)
     * 
     * @note Bei externem Aufruf kann es Timing-Probleme geben
     */
    void cycle();


private:
    void initTime();
    int IDtoCar(int CarID);
    int reverseBits(int value, int length);
    void changeList(int Nr, int value, int carNr);
    void pushWord(int CPin);
    void changeBits(int carNr, int value, int shifts, int mask);
    int Cpin;
    bool Crun;


};

#endif