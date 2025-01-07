#ifndef Settings_h
#define Settings_h

/**
 * @file Settings.h
 * 
 *
 * @brief In dieser Datei sind alle veränderbaren Einstellungen zentral abgespeichert.
 * 
 * 
 */

  //##################//
 //  Car-Parameters  //
//##################//


/**
 * @name Ein-/Ausfahrt von Fahrzeugen
 */
/**@{*/

/**
 * @brief Geschwindigkeit mit der das Fahrzeug bei der Einfahrt bis zur CarDetectionUnit fahren soll; 0-15. 
 */
#define VEL_CarEntry 5

/// @brief Geschwindigkeit, mit der das Fahrzeug bei der Einfahrt von der CarDetectionUnit bis zum Stopper fahren soll; 0-15
#define VEL_CarEntry_brake 2

/// @brief Geschwindigkeit, mit der das Fahrzeug aus dem Lager automatisiert ausfahren soll; 0-15
#define VEL_CarExit 15

/**@}*/

  //#####################//
 //  Fahrzeugerkennung  //
//#####################//

///@name Parameter CarDetectionUnit
///@{

///@brief Erlaubte zeitliche Abweichung der Impulse von der Soll-Periodendauer ;  in µs - Maximum: 32µs
#define IR_DECT_Period_Tolerance 10         

///@brief Minimale Zeitdauer zwischen zwei Impulsen
#define IR_DECT_Period_min 64

///@brief Minimale Zeitdauer zwischen zwei durchfahrenden Autos in µs
#define IR_DECT_CarTimeGap 32000

///@}


  //#############################//
 //  Debugging-Functionalities  //
//#############################//

/// @name Debugging-Funktionalitäten
///@{

/// @brief Helligkeit der Debug-LED des Mikrocontrollers
#define DEBUG_RGB_BRIGHTNESS 255


/// @brief Aktiviere Debugging über Serielle Schnittstelle UART0. Wird durch Auskommentieren deaktiviert. Hat, wenn aktiviert, stets Vorrang vor Telnet-Debugging.

#define SERIAL_DEBUGGING

/// @brief Aktiviere Over-the-Air-Flashing und Debugging, wird durch auskommentieren deaktiviert
#define OTA_Flashing

/// @brief Aktiviere Debugging über Telnet, nur möglich, wenn SERIAL_DEBUGGING nicht aktiv und OTA_FLASHING aktiviert ist.
#define TELNET_DEBUGGING

/**
 * @name Netzwerkeinstellungen
 */
/**@{*/
#define OTA_SSID "Labore-Hof AT-MT"   ///< WLAN-Name
#define OTA_PW "laborwlan"            ///< WLAN-Passwort
#define OTA_IP {192,168,9,5}          ///< Statische IP-Adresse
#define OTA_DNS {192,168,9,1}         ///< DNS-Server
#define OTA_SUBNET {255,255,255,0}    ///< Subnet-Maske
#define OTA_GW {192,168,9,1}          ///< Gateway

/**@}*/
///@}



  //##############################//
 //  Serial Connection with PLC  //
//##############################//

/// @name Kommunikation mit der SPS
/// Serielle Kommunikation über RS-485 mit eigenem Protokoll-

///@{
/// @brief Aktiviert Verbindungsüberwachung zur SPS
#define SPS_Connected
/// @brief Kommunikationstimeout zur SPS in s.          
#define SPS_UART_Timeout 30

/// @name Kommunikationsprotokoll

/// @brief Grösse des Befehlsspeichers der von der SPS zu empfangeneden Befehle. Muss mindestens der höchsten Befehlsnummer entsprechen.
#define SPS_UART_RxCommandMemory 85       

///@brief Pollingfrequenz der SPS; Hz.
#define SPS_UART_Frequency 10
             
///@brief Datenpuffer für Daten, die zur SPS gesendet werden sollen; Anzahl Pakete
#define C_MC_DataBufferSize 10  

///@}

/// @name Pakete CarControlUnit --> SPS
/// @brief Definitionen der Pakete, die die CarControlUnit an die SPS sendet
///    Paketgröße: 2 Bytes: 1 Befehlsbyte, 1 Datenbyte
///    
///@{

/**
 * @brief Lifesignal der CarControlUnit
 * Wird als Antwort auf das Polling-Signal der SPS gesendet, sofern sich keine anderen Daten im Sendepuffer befinden
 */
#define C_MC_LIFESIGNAL 0,1
/**
 * @brief Datenpaket: Fahrzeug in Boxengass eingefahren
 * @param value Reglernummer des eingefahrenen Fahrzeugs
 */
#define C_MC_CarIN(value) 1,value
/**
 * @brief Datenpaket: Fahrzeug aus Boxengasse ausgefahren
 * @param value Reglernummer des ausgefahrenen Fahrzeugs
 */
#define C_MC_CarOUT(value) 2,value
/**
 * @brief Datenpaket: Fahrzeug fertig programmiert
 * @param value Reglernummer des programmierten Fahrzeugs
 */
#define C_MC_CarPROGRAMMED(value) 3,value
/**
 * @brief Bestätigungspaket: Von der SPS erhaltenes Befehlspaket verstanden (ACK).
 * @param value Wert des Datenbyte
 */
#define C_MC_OK(value) 80,value     //Ok Packet MC->SPS

///@}

///@name Pakete SPS --> CarControlUnit
/// Definitionen der Pakete, die die SPS an die CarControlUnit sendet
///    Paketgröse: 5 Bytes: 1 Befehlsbyte, 4 Datenbytes
///
///@{

/**
 * @brief Pollingpaket: Wird von der SPS regelmäßig gesendet, um die CarControlUnit und die SPS zu synchronisieren und gleichzeitig Daten von der CarControlUnit zu pollen.
 */
#define C_SPS_SYNC {0,0x79,0x9a,0x62,0x43}         //Polling- and Sync-Signal from SPS -> MC

/**
 * @brief Befehlspaket: Fahrzeug programmieren
 * @param id Reglernummer des Fahrzeugs
 * @param vmax Maximalgeschwindigkeit des Fahrzeugs
 * @param brake Bremsverhalten des Fahrzeugs
 * @param fuel Treibstoffverhalten des Fahrzeugs
 * 
 */
#define C_SPS_PROGRAM {1, id, vmax, brake, fuel}    //Program-Command from SPS -> MC
    #define C_SPS_PROGRAM_ID 1      ///< Index im Paket: Reglernummer des Fahrzeugs
    #define C_SPS_PROGRAM_VMAX 2    ///< Index im Paket: Maximalgeschwindigkeit des Fahrzeugs
    #define C_SPS_PROGRAM_BRAKE 3   ///< Index im Paket: Bremsverhalten des Fahrzeugs
    #define C_SPS_PROGRAM_FUEL 4    ///< Index im Paket: Treibstoffverhalten des Fahrzeugs

/**
* @brief Befehlspaket: Fahrzeug ausfahren
* @param id Reglernummer des Fahrzeugs
*/
#define C_SPS_CarDRIVE {0x05, id, na, na, na}  //Command to drive a car back to the course
    #define C_SPS_CarDRIVE_ID 1     ///< Index im Paket: Reglernummer des Fahrzeugs

///@}




#endif