#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Settings.h>

  //################//
 //  Definitionen  //
//################//

//In dieser Datei sollten alle Definitionen abgespeichert werden
//An diesen soll nicht mehr herumgepfuscht werden, sonst kann der Code unbrauchbar werden!!

//##########################################################################################//

//Fahrzeugerkennung mittels IR_Fototransistor
#define IR_SENSOR_PIN 21


//Serielle Schnittstelle SPS RS-485
#define SPS_UART_NUM 0    //Nummer des UART-Ausgangs 0,1,2
#define SPS_UART_RX 17
#define SPS_UART_TX 16

#define SPS_UART_RxPacketLength 5       //Length of Packets to receive form SPS
#define SPS_UART_TxPacketLength 2       //Length of Packets to send to SPS


  //#################//
 // Compiler-Macros //
//#################//

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
#define STR_HELPER(x) #x 
#define STR(x) STR_HELPER(x) 

//Serielles Debugging
//-Alles was nur im Debug-Fall ausgegeben werden soll, kann mit DEBUG(...) definiert werden
#ifdef SERIAL_DEBUGGING
#define DEBUG(...) Serial.println(STR(__VA_ARGS__) ); 
#define DEBUGF(...) Serial.printf(__VA_ARGS__); 
#else 
#define DEBUG(...) 
#define DEBUGF(...) 
#endif


//Serial Periods
#define SPS_UART_Period 1000000/SPS_UART_Frequency

#endif