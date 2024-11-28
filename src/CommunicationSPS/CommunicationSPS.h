#ifndef CommunicationSPS_h
#define CommunicationSPS_h
#include <Arduino.h>
#include <Definitions.h>
#include "AnswerMC.h"

typedef void (*command_function)(uint8_t *);            // function (uint8_t* pointer to buffer)

extern void comSPS_protocol();  //configures the SPS-Rx Protocol
void comSPS_addCommand(uint8_t cmd, command_function function);
void comSPS_execute();

void comSPS_init();
void comSPS_sync();




#endif