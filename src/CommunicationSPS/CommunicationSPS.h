#ifndef CommunicationSPS_h
#define CommunicationSPS_h
#include <Arduino.h>
#include <Definitions.h>

typedef void (*command_function)(uint8_t *);            // function (uint8_t* pointer to buffer)

extern void comSPS_protocol();  //configures the SPS-Rx Protocol
void comSPS_addCommand(uint8_t cmd, command_function function);
void comSPS_execute();

void comSPS_init();
void comSPS_sync();

void comSPS_send2(uint8_t cmd, uint8_t data);
//Add Data to SPS-Send-Data-Buffer
void comSPS_writeData(uint8_t cmd, uint8_t data);
//Answer to request from SPS. Sends data if available, else answers with MC-Lifesignal
void comSPS_sendDataPacket();                       
#endif