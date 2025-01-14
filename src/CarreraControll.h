#ifndef CARRERACONTROLL_H
#define CARRERACONTROLL_H

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
 * @brief die Klasse CarreraControll übernimmt alle ansteuerungs Funktionen der Carrera Autos
 * Die Klasse wird momentan nur einmal verwendet. Die Klasse wurde am Anfang des Projektes erstellt, 
 * allerdings könnte sie überarbeitet werden, um mehrere Treiber auf einer CarControllUnit zu betreiben.
 * @note die Klasse ist nur einmal erstellt und nicht weiter getestet (außer Einzelbetrieb)
 */
class CarreraControll {
public:

    CarreraControll();
    static void IRAM_ATTR timerInterrupt();


    /**
     * @brief Initallisieren der CarreraControll Bibliothek
     * 
     * @param pin Ausgabepinn der CarreraControll Bibliothek
     */
    void conf(int pin);



    /**
     * @brief Setzt die ID eines Fahrzeugs auf mitgegebene id
     * 
     * @param carID Die ID die Auf das mommentane Fahrzeug gesetzt werden soll
     */
    void setID(int carID);


    /**
     * @brief Frage ab ob das Programieren einer ID erfolgreich durch gelaufen ist
     * @details diese Funktion wird genau wie die Rückmeldung des programmierens nur einmalig zurückgegeben
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
     * @param carNr die zu programmierende ID
     * @param maxSpeed die zu programmierende Maximalgeschwindigeit
     * @param brake die zu programmierende Bremskraft
     * @param fuel die zu programmierende Tankgröße
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
     * @brief zyklus klasse (sollte nicht von außen aufgerufen werden muss aber public bleiben wegen den Interrupts)
     * 
     * @note bei externem aufruf kann es timing probleme geben
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