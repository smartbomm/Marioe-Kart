#ifndef AnswerMC_h
#define AnswerMC_h

#include <Arduino.h>
#include <Settings.h>

void comSPS_send2(uint8_t cmd, uint8_t data){
    byte buffer[2] = {cmd, data};
    SPS_UART.write(buffer, 2);
}
#endif