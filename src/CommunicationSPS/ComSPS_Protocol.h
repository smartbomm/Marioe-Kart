#ifndef ComSPS_Protocol_h
#define ComSPS_Protocol_h

#include <CommunicationSPS/Command.h>
#include <Settings.h>

Command SPSCommand(SPS_UART_RxPacketLength);
Command MCCommand (SPS_UART_TxPacketLength);

void initializeProtocol() {
    SPSCommand.add(0, lifeSignal);
    SPSCommand.add(1, programCar);
    SPSCommand.add(5, drive);

}

void lifeSignal(byte * buffer, uint8_t word_size) {

}

void programCar(byte * buffer, uint8_t word_size) {
    
}

void drive(byte * buffer, uint8_t word_size) {
    
}

#endif