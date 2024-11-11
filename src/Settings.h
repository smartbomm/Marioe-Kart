#ifndef Settings_h
#define Settings_h

#include <Definitions.h>
#include <CompilerMacros.h>

//In dieser Datei sollen alle veränderbaren Einstellungen abgespeichert werden

//Enable Serial Debugging
#define SERIAL_DEBUGGING


//Serielle Kommunikation
#define SPS_UART_RxPacketLength 5 //Length of Packets to receive form SPS
#define SPS_UART_TxPacketLength 2 //Length of Packets to send to SPS

#define SPS_UART_Frequency 10     //Communication Frequency in Hz

//Pakete µC
#define C_LIFESIGNAL_MC {0,0}
#define C_OK_MC {80,0}

//Pakete SPS
#define C_LIFESIGNAL_SPS {48,48,48,48,48}

#endif