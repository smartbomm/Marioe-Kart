/**
 * @file CarDetection.h
 * @author Joel Bommeli (joel.bommeli@hof-university.de)
 * @version 0.1
 * @date 2024-12-28
 * @copyright Copyright (c) 2024
 * 
 * 
 * @brief Programmteil zur Fahrzeugerkennung mittels IR_Fototransistor
 * 
 * Jedes Carrera Digital 124 - Fahrzeug ist mit einer IR-LED ausgerüstet. Je nach zugeordnetem Regler, blinkt diese mit einer anderen Frequenz. Die Sende-Frequenzen sind folgende:
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
 * Ein Impuls am deklarierten Pin löst eine Interrupt-Routine aus. Diese speichert die aktuelle Systemzeit ab. Beim nächsten Interrupt, wird ebenfalls die Systemzeit abgespeichert und der Interrupt deaktiviert. 
 * Der Interrupt wird erst wieder reaktiviert, wenn die Zeitdifferenz durch die Funktion `carDectx_Execute()`ausgewertet wurde und das Mindestintervall zwischen zwei durchfahrenden Fahrzeugen gewährleistet wurde.
 * Aufgrund von Zeitmangel wurden die Funktionen für die zwei Infrarot-Sensoren einfach kopiert und tragen jeweils das Präfix carDect1... bzw. carDect2... . 
 * Eine Möglichkeit der Weiterentwiscklung bestände darin, eine Struktur zu entwerfen, in der jeweils die Parameter, der Sensorik hinterlegt ist, so dass beide Sensoren mit der gleichen Funktion bedient werden könnten.
 * 
 * A link to the define #VEL_CarExit
 * 
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

//to be executed in program loop, calculates car ids, if cars where detected and writes to buffer
//returns detected Car
//If no car was detected returns 99

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