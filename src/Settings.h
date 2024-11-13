#ifndef Settings_h
#define Settings_h

#include <Definitions.h>
#include <CompilerMacros.h>

//In dieser Datei sollen alle veränderbaren Einstellungen abgespeichert werden

//Enable Serial Debugging
#define SERIAL_DEBUGGING


//Serielle Kommunikation
#define SPS_UART_RxCommandMemory 85       //Number of Commands that can be stored on MC, \n must be the highest command number actually
#define SPS_UART_RxPacketLength 5       //Length of Packets to receive form SPS
#define SPS_UART_TxPacketLength 2       //Length of Packets to send to SPS


#define SPS_UART_Frequency 10           //Communication Frequency in Hz

//Pakete µC
#define C_LIFESIGNAL_MC {0,1}
#define C_OK_MC {80,0}

//Pakete SPS
#define C_SPS_SYNC {0,0x79,0x9a,0x62,0x43}         //Packet SPS will send to syncSPS and µC
#define C_LIFESIGNAL_SPS {48,48,48,48,48}

#endif