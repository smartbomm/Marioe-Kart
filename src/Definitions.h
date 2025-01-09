#ifndef DEFINITIONS_H
#define DEFINITIONS_H


/**
 * @file Definitions.h
 * @brief @brief In dieser Datei sind alle globalen Parameter abgespeichert. Dies umfasst Pinkonfigurationen Definition der UART-Interfaces sowie Makros für Debugging.
 * @warning Werden Parameter in dieser Datei verändert, kann der Code unbrauchbar werden, sodass das Programm nicht mehr funktioniert. Alle im Betrieb änderbaren Parameter sind in Settings.h abgelegt.
 * 
 * 
 * @author your name (you@domain.com)
 * @version 0.1
 * @date 2025-01-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//##########################################################################################//

#include <Settings.h>
#include <TelnetStream.h>


  //########################//
 ///  @name Definitionen  
//########################//

/// @{

#define ESP32_RTOS  ///<RTOS-Modus aktivieren
#define RGB_LED 38  ///<Debugging-RGB-LED an Pin 38

//Fahrzeugerkennung mittels IR_Fototransistor

#define CAR_Detect_EntryLane_p 12 ///<Pin für CarDetectionUnit Einfahrt
#define CAR_Detect_ExitLane_p 13  ///<Pin für CarDetectionUnit Ausfahrt



//Serielle Schnittstelle SPS RS-485
#define SPS_UART_NUM 1    ///<Nummer des UART-Ausgangs 0,1,2. Kann fürs Debugging auf 0 gesetzt werden, um die mit USB verbundene serielle Schnittstelle zu verwenden.
#define SPS_UART_RX_p 11  ///<Pin für UART-RX
#define SPS_UART_TX_p 10  ///<Pin für UART-TX

#define SPS_UART_RxPacketLength 5       ///<Länge der Pakete, die von der SPS empfangen werden
#define SPS_UART_TxPacketLength 2       ///<Länge der Pakete, die an die SPS gesendet werden


/* Relaisumschaltung Bahn
 *  FALSE == Spannungsversorgung von externer Carrerabahn
 *  TRUE  == Spannungsversorgung von interner Steuerung 
 * 
*/
#define RELAY_ExitLane_p 41   ///<Pin des Relais für Umschaltung der Spannungsversorgung der Ausfahrtsschiene
#define RELAY_EntryLane_p 42  ///<Pin des Relais für Umschaltung der Spannungsversorgung der Einfahrtsschiene

//Bahnkommunikation
#define DRIVER_ProgLane_p 40  ///<Pin für den Bahntreiber der Programmierschiene

///@}




   //#########################//
  /// @name Debugging-Makros 
 //#########################//

///@{

#if SPS_UART_NUM == 0
#define SPS_UART_EVENT serialEvent
#define SPS_UART Serial
#elif SPS_UART_NUM == 1
#define SPS_UART_EVENT serialEvent1
#define SPS_UART Serial1
#elif SPS_UART_NUM == 2
#define SPS_UART_EVENT serialEvent2
#define SPS_UART Serial2
#endif

//Strings verketten
#define STR_HELPER(x) #x  ///<Hilfsfunktion für STR
#define STR(x) STR_HELPER(x)  ///<Strings verketten

//Serielles Debugging
//-Alles was nur im Debug-Fall ausgegeben werden soll, kann mit DEBUG(...) definiert werden



#ifdef SERIAL_DEBUGGING
/**
 * @brief Ausgabe von Text auf der definierten Debugging-Schnittstelle (UART oder Telnet)
 * @param ... Text, der ausgegeben werden soll, ohne ""
 */
#define DEBUG(...) Serial.println(STR(__VA_ARGS__) ); 
/**
 * @brief Ausgabe von formatiertem Text auf der definierten Debugging-Schnittstelle (UART oder Telnet)
 * @param ... Text, der ausgegeben werden soll, Syntax analog zu printf
 */
#define DEBUGF(...) Serial.printf(__VA_ARGS__); 
#elif defined TELNET_DEBUGGING
/**
 * @brief Ausgabe von Text auf der definierten Debugging-Schnittstelle (UART oder Telnet)
 * @param ... Text, der ausgegeben werden soll, ohne ""
 */
#define DEBUG(...) TelnetStream.println(STR(__VA_ARGS__));
/**
 * @brief Ausgabe von formatiertem Text auf der definierten Debugging-Schnittstelle (UART oder Telnet)
 * @param ... Text, der ausgegeben werden soll, Syntax analog zu printf
 */
#define DEBUGF(...) TelnetStream.printf(__VA_ARGS__); 
#else
#define DEBUG(...) 
#define DEBUGF(...) 
#endif

///@}

#endif