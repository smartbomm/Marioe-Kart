#ifndef Settings_h
#define Settings_h

//In dieser Datei sollen alle veränderbaren Einstellungen abgespeichert werden

  //##################//
 //  Car-Parameters  //
//##################//

//Entry velocity before Car-Detection-Unit
#define VEL_CarEntry 5
//Entry velocity after Car-Detection-Unit
#define VEL_CarEntry_brake 2
//Exit velocity
#define VEL_CarExit 15


  //#############################//
 //  Debugging-Functionalities  //
//#############################//

#define DEBUG_RGB_BRIGHTNESS 255

//if defined, enable Serial Debugging
//#define SERIAL_DEBUGGING

//if defined, enable Over-the-Air-Flashing
#define OTA_Flashing

//Network-Settings for OTA-Functionality
#define OTA_SSID "Labore-Hof AT-MT"
#define OTA_PW "laborwlan"
#define OTA_IP {192,168,9,5}
#define OTA_DNS {192,168,9,1}
#define OTA_SUBNET {255,255,255,0}
#define OTA_GW {192,168,9,1}

//if defined, enable Telnet Debugging, only possible if Serial Debugging disabled and OTA_Flashing is enabled
#define TELNET_DEBUGGING

  //##############################//
 //  Serial Connection with PLC  //
//##############################//

//Enables Watchdog for PLC-Connection
#define SPS_Connected
//Communication Watchdog Timeout in s           
#define SPS_UART_Timeout 30

//Number of Commands that can be stored on MC, must be at least the highest command id
#define SPS_UART_RxCommandMemory 85       

//Communication Frequency in Hz
#define SPS_UART_Frequency 10
             
//Size of Output-Buffer for data to bes send to PLC (Amount of data-packets)
#define C_MC_DataBufferSize 10  

//Data Packets µC --> PLC
#define C_MC_LIFESIGNAL 0,1
#define C_MC_CarIN(value) 1,value
#define C_MC_CarOUT(value) 2,value
#define C_MC_CarPROGRAMMED(value) 3,value
#define C_MC_OK(value) 80,value     //Ok Packet MC->SPS

//Data Packets PLC --> µC
#define C_SPS_SYNC {0,0x79,0x9a,0x62,0x43}         //Life- and Sync-Signal from SPS -> MC

//Definitions of commands PLC can send to µC
#define C_SPS_PROGRAM {1, id, vmax, brake, fuel}    //Program-Command from SPS -> MC
    #define C_SPS_PROGRAM_ID 1      //Index of Id to program
    #define C_SPS_PROGRAM_VMAX 2    //Index of vmax-parameter
    #define C_SPS_PROGRAM_BRAKE 3   //Index of brake-parameter
    #define C_SPS_PROGRAM_FUEL 4    //Index of fuel-parameter
#define C_SPS_CarDRIVE {0x05, id, na, na, na}  //Command to drive a car back to the course
    #define C_SPS_CarDRIVE_ID 1     //Index of Id to drive
#define C_SPS_CarDRIVE {0x05, id, na, na, na}  //Command to drive a car back to the course
    #define C_SPS_CarDRIVE_ID 1     //Index of Id to drive

  //#####################//
 //  Fahrzeugerkennung  //
//#####################//

#define IR_DECT_Period_Tolerance 10         //Erlaubte zeitliche Abweichung in µs von der Soll-Periodendauer - Maximum: 32µs
#define IR_DECT_Period_min 64               //Minimale Zeitdauer zwischen zwei Pulsen

#define IR_DECT_CarTimeGap 32000            //Minimale Zeitdauer zwischen zwei durchfahrenden Autos in µs

#define IR_DECT_BufferSize 5                //Anzahl der erkannnten Autos, die auf dem Mikrocontroller zwischengespeichert werden sollen
#define IR_DECT_Counts 1


#endif