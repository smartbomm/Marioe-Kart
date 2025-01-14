/**
 * @page Fahrzeugerkennung
 * 
 * ## Carrera Fahrzeugidentifizierung
 * 
 * Jedes Carrera Digital 124 - Fahrzeug ist mit einer IR-LED ausgerüstet. Je nach zugeordnetem Regler, blinkt diese mit einer anderen Frequenz. Dies dient auf der Orginal-Rennbahn dazu, an den Weichen und den Rundenzählern die Fahrzeuge zu erkennen.
 * Im Projekt wurde diese Fahrzeugerkennung zur Erkennung der Carrera-Autos bei der Ein-undAusfahrt in das automatisierte Lager verwendet. Dazu wurde eine spezielle Hardware designt, die in eine Schiene eingebaut werden kann. 
 * Es handelt sich hierbei um die bereits erwähnte CarDetectionUnit. Sie enthält einen IR-Fototransistor, sowie eine analoge Auswertungsschaltung, die das Signal verbessert.
 * 
 *  Die Sende-Frequenzen sind folgende :
 * 
 * 
 * Regler-Nummer  | Sendefrequenz in kHz  | Periodendauer in µs
 * -------------- | --------------------- | --------------------
 *             1  | 15,625                | 64
 *             2  | 7,813                 | 128
 *             3  | 5,208                 | 192
 *             4  | 3,906                 | 256
 *             5  | 3,125                 | 320
 *             6  | 2,604                 | 384
 *      Ghostcar  | 2,232                 | 448
 *       Pacecar  | 1,953                 | 512
 * 
 * Quelle: http://wasserstoffe.de/carrera-hacks/infrarot-erkennung/index.html 
 * 
 * Das Modul @ref CarDetection.h "CarDetection" dient zur Erkennung der entsprechenden Reglernummer anhand der Periodendauer. 
 * 
 * ## Funktion des Programms
 * Ein Impuls am deklarierten Pin löst eine Interrupt-Routine aus. Diese speichert die aktuelle Systemzeit ab. Beim nächsten Interrupt, wird ebenfalls die Systemzeit abgespeichert und der Interrupt deaktiviert. 
 * Der Interrupt wird erst wieder reaktiviert, wenn die Zeitdifferenz durch die Funktion `carDectx_execute()`ausgewertet wurde und das Mindestintervall zwischen zwei durchfahrenden Fahrzeugen gewährleistet wurde.
 * 
 * @note Aufgrund von Zeitmangel wurden die Funktionen für die zwei Infrarot-Sensoren einfach kopiert und tragen jeweils das Präfix carDect1... bzw. carDect2... . Eine Möglichkeit der Weiterentwicklung bestände darin, eine Struktur zu entwerfen, in der jeweils die Parameter, der Sensorik hinterlegt ist, so dass beide Sensoren mit der gleichen Funktion bedient werden könnten.
 * 
 * #### Aufbau der ISR am Beispiel von `carDect1_isr()`
 * 
 * Diese Funktion wird durch einen Interrupt ausgelöst, wenn ein Impuls am Pin erkannt wird.
 * 
 * @snippet{lineno} CarDetection.cpp carDect1_isr
 * 
 * #### Auswertung der Reglernummer am Beispiel von `carDect1_execute()`
 * 
 * Diese Funktion muss in jedem Programmzyklus aufgerufen werden, um die Reglernummer auszuwerten, bzw. den Interrupt wieder zu aktivieren.
 * 
 * @snippet{lineno} CarDetection.cpp carDect1_exec  
 * 
 * Genaueres zur Verwendung der Funktionen ist in der Dokumentation zu CarDetection.h zu finden.
 * 
 */

/**
 * @file CarDetection.h
 * @author Joel Bommeli (joel.bommeli@hof-university.de)
 * @version 0.1
 * @date 2024-12-28
 * @copyright Copyright (c) 2024
 * 
 * 
 * @brief Programmteil zur Fahrzeugerkennung mittels CarDetectionUnit. @see Fahrzeugerkennung
 * 
 * Die Funktionsweise der Fahrzeugerkennung ist auf der Seite @ref Fahrzeugerkennung "Fahrzeugerkennung" dokumentiert.
 */

#ifndef CARDETECTION_H
#define CARDETECTION_H
#include <Arduino.h>
#include "Definitions.h"

//Configuration of the CarDetectionUnit
//- initialize pin and variables

/// @brief Initialisiere Eingänge und Interrupts der CarDetectionUnit 1.
/// @param pin Pin des Mikrocontroller, an dem die CarDetectionUnit angeschloßen ist
void carDect1_init (uint8_t pin);   

/// @brief Initialisiere Eingänge und Interrupts der CarDetectionUnit 2.
/// @param pin Pin des Mikrocontroller, an dem die CarDetectionUnit angeschloßen ist
void carDect2_init (uint8_t pin); 

/// @brief Abfrage, ob an der CarDetectionUnit 1 seit der letzten Ausführung ein Fahrzeug erkannt wurde. Falls ja, wird dessen zugeordneter Regler gemäß obenstehender Tabelle berechnet.
/// @return ID, bzw Reglerkanal, dem das erkannte Fahrzeug zugeordnet ist. Wurde kein gültiger Wert erkannt, ist die Rückgabe 99;
uint8_t carDect1_execute();

/// @brief Abfrage, ob an der CarDetectionUnit 2 seit der letzten Ausführung ein Fahrzeug erkannt wurde. Falls ja, wird dessen zugeordneter Regler gemäß obenstehender Tabelle berechnet.
/// @return ID, bzw Reglerkanal, dem das erkannte Fahrzeug zugeordnet ist. Wurde kein gültiger Wert erkannt, ist die Rückgabe 99;
uint8_t carDect2_execute();

//Interrupt-Routine saves the period between two pulses for calculating the car-id

/// @brief Interrupt Routine zur Berechnung der Periodendauer zwischen zwei erkannten Impulsen
void IRAM_ATTR carDect1_isr(); 

/// @brief Interrupt Routine zur Berechnung der Periodendauer zwischen zwei erkannten Impulsen
void IRAM_ATTR carDect2_isr();  

#endif 