#include <CommunicationSPS/CommunicationSPS.h>


command_function comSPS_commands[SPS_UART_RxCommandMemory]; //Array storing function_pointers for each configured SPS-Command
bool comSPS_commandsDefined [SPS_UART_RxCommandMemory] = {false};     //Array storing which commands are defined
uint8_t comSPS_packetSize = SPS_UART_RxPacketLength;          //Size of one SPS-Command-Packet
byte comSPS_receivedPacket [SPS_UART_RxPacketLength];          //Buffer to save one received packet


//Configuration of Serial Communications with SPS
//- Configuration of UART Interface
//- Initialize protocol of communication
void comSPS_init(){
    SPS_UART.begin(9600, SERIAL_8N1, SPS_UART_RX, SPS_UART_TX);
    comSPS_protocol();
}

//Syncronize Serial Communications between SPS and µC
//- blocking until first valid life Signal comes from SPS
void comSPS_sync()
{
    byte syncPacket[SPS_UART_RxPacketLength] = C_SPS_SYNC;
    uint8_t packetCounter = 0;              // Check for 5 zeroes in a row for life packet
    while (packetCounter< 5) {       // Blocking until valid Life Signal from SPS is received
        if (SPS_UART.available() > 0)
        {
            if (SPS_UART.read() == syncPacket[packetCounter])
            {
                Serial.printf("Ok: %d", packetCounter);
                packetCounter++;
            }
            else
                packetCounter = 0;
        }
    }
    DEBUG(Connection to SPS established!);
}

//Add SPS-Command and linked routine
void comSPS_addCommand(uint8_t cmd, command_function function){ 
    comSPS_commands[cmd] = function;
    comSPS_commandsDefined[cmd] = true;
}

//Periodic function to read Serial Data from SPS and execute linked function
void comSPS_execute(){
    if(SPS_UART.available()>=SPS_UART_RxPacketLength){
        
        SPS_UART.read(comSPS_receivedPacket, SPS_UART_RxPacketLength);
        if(comSPS_commandsDefined[comSPS_receivedPacket[0]]){    //Check if received command is defined and valid
            comSPS_commands[comSPS_receivedPacket [0]](comSPS_receivedPacket);
        } 
        #ifdef SERIAL_DEBUGGING
            Serial.printf("Paket empfangen: %X %X %X %X %X\n", comSPS_receivedPacket[0], comSPS_receivedPacket[1], comSPS_receivedPacket[2], comSPS_receivedPacket[3], comSPS_receivedPacket[4]);
        #endif
    }
}



