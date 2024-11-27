#ifndef AnswerMC_h
#define AnswerMC_h

#include <Arduino.h>
#include <Settings.h>

uint8_t ansMc_dataBuffer [C_MC_DataBufferSize*2] = {0};
uint8_t ansMc_nextRead;
uint8_t ansMc_nextWrite;

void ansMc_init() {
    ansMc_nextRead = 0;
    ansMc_nextWrite = 0;
}


void comSPS_send2(uint8_t cmd, uint8_t data){
    byte buffer[2] = {cmd, data};
    SPS_UART.write(buffer, 2);
}
//Add Data to SPS-Send-Data-Buffer
void comSPS_writeData(uint8_t cmd, uint8_t data){
    ansMc_dataBuffer[ansMc_nextWrite] = cmd;
    ansMc_dataBuffer[ansMc_nextWrite + 1] = data;
    if(ansMc_nextWrite < C_MC_DataBufferSize-2) ansMc_nextWrite+=2; //Move pointer to next value in FIFO-Buffer
    else ansMc_nextWrite = 0;
}

//Answer to request from SPS. Sends data if available, else answers with MC-Lifesignal
void comSPS_sendData(){   
    //SPS_UART.write(C_ClientID);               Wieder aktivieren wenn die SPS auf Clients reagiert
    if(ansMc_nextRead != ansMc_nextWrite) {     //if available, send data
        SPS_UART.write(&ansMc_dataBuffer[ansMc_nextRead], 2);
        if(ansMc_nextRead < C_MC_DataBufferSize-2) ansMc_nextRead+=2; //Move pointer to next value in FIFO-Buffer
        else ansMc_nextRead = 0;
    } else {    //send lifesignal
        uint8_t buffer [2] = {0,1};
        SPS_UART.write(buffer, 2);
    }
    
    
}
#endif