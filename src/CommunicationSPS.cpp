#include <CommunicationSPS.h>
#include "esp32s2/rom/uart.h"
#include <Settings.h>

byte receivedBuffer[5*SPS_UART_RxPacketLength];   //Buffer for received Commands with space for 5 Packets
uint8_t receivedBufferIndex = 0;
bool newDataReceived;
uint32_t timeDataReceived;

byte sendBuffer [5*SPS_UART_TxPacketLength] = {0};   //Buffer for received Commands with space for 5 Packets
uint8_t sendBufferIndex = 0;
byte send_lifesignal_mc[2] = C_LIFESIGNAL_MC;
byte send_ok_mc[2] = C_OK_MC;






//- Configuration of UART Interface,
//- blocking until first valid life Signal comes from SPS,
void initializeCom()
{
    SPS_UART.begin(9600, SERIAL_8N1);
    byte cmd[2];
    bool sps_lifesignal = false;
    while (!sps_lifesignal)
    { // Blocking until valid Life Signal from SPS is received
        if (SPS_UART.available() >= SPS_UART_RxPacketLength && SPS_UART.read() == 48)   //Get first 0 received
        {
            uint8_t packetCounter = 1;
            while (packetCounter < 5)   //Check for 4 more zeroes for valid life packet
            {
                if (SPS_UART.read() == 48){
                    packetCounter++;
                    if(packetCounter == 5)sps_lifesignal = true;
                }
                else if(SPS_UART.available()>0)
                    packetCounter = 0;
            }
        }
    }

    #ifdef SERIAL_DEBUGGING
    Serial.println("Connection to SPS established!");
    #endif
}


uint8_t command(byte * packet) {
    return packet [0];

}

void answer(){
    if(newDataReceived && micros() - timeDataReceived< SPS_UART_Period/2){
        if(command(&receivedBuffer[receivedBufferIndex]) == 0) {
            SPS_UART.write(sendBuffer, SPS_UART_TxPacketLength);
        } else {
            SPS_UART.write(send_ok_mc, SPS_UART_TxPacketLength);
        }
    }
    
}
