#include <CommunicationSPS/CommunicationSPS.h>

//#########################################
//Receiving Data from SPS
//########################################

command_function comSPS_commands[SPS_UART_RxCommandMemory]; //Array storing function_pointers for each configured SPS-Command
bool comSPS_commandsDefined [SPS_UART_RxCommandMemory] = {false};     //Array storing which commands are defined
uint8_t comSPS_packetSize = SPS_UART_RxPacketLength;          //Size of one SPS-Command-Packet
byte comSPS_receivedPacket [SPS_UART_RxPacketLength];          //Buffer to save one received packet


//Configuration of Serial Communications with SPS
//- Configuration of UART Interface
//- Initialize protocol of communication
void comSPS_init(){
    #if (SPS_UART_NUM == 0)
    SPS_UART.begin(9600);
    #else
    SPS_UART.begin(9600, SERIAL_8N1, SPS_UART_RX, SPS_UART_TX);
    #endif
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
            uint8_t received = SPS_UART.read();
            if (received == syncPacket[packetCounter])
            {
                DEBUGF("Ok: %d, ", packetCounter);
                packetCounter++;
            }
            else
                packetCounter = 0;
                DEBUGF("%d ,", received);
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
        DEBUGF("Receive Packet: %02X %02X %02X %02X %02X\t", comSPS_receivedPacket[0], comSPS_receivedPacket[1], comSPS_receivedPacket[2], comSPS_receivedPacket[3], comSPS_receivedPacket[4]);
        if(comSPS_commandsDefined[comSPS_receivedPacket[0]]){    //Check if received command is defined and valid
            comSPS_commands[comSPS_receivedPacket [0]](comSPS_receivedPacket);
        } 
    }
}

//#########################################
//Sending Data to SPS
//########################################

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
    DEBUGF("Send Packet: %02X %02X\n", buffer [0], buffer [1]);
}
//Add Data to SPS-Send-Data-Buffer
void comSPS_writeData(uint8_t cmd, uint8_t data){
    ansMc_dataBuffer[ansMc_nextWrite] = cmd;
    ansMc_dataBuffer[ansMc_nextWrite + 1] = data;
    if(ansMc_nextWrite < C_MC_DataBufferSize-2) ansMc_nextWrite+=2; //Move pointer to next value in FIFO-Buffer
    else ansMc_nextWrite = 0;
}

//Answer to request from SPS. Sends data if available, else answers with MC-Lifesignal
void comSPS_sendDataPacket(){   
    //SPS_UART.write(C_ClientID);               Wieder aktivieren wenn die SPS auf Clients reagiert
    if(ansMc_nextRead != ansMc_nextWrite) {     //if available, send data
        SPS_UART.write(&ansMc_dataBuffer[ansMc_nextRead], 2);
        DEBUGF("Send Packet: %02X %02X\n", ansMc_dataBuffer [ansMc_nextRead], ansMc_dataBuffer [ansMc_nextRead + 1]);
        if(ansMc_nextRead < C_MC_DataBufferSize-2) ansMc_nextRead+=2; //Move pointer to next value in FIFO-Buffer
        else ansMc_nextRead = 0;
    } else {    //send lifesignal
        uint8_t buffer [2] = {0,1};
        SPS_UART.write(buffer, 2);
        DEBUGF("Send Packet: %02X %02X\n", buffer [0], buffer [1]);
    }
    
    
}



