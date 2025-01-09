#ifndef CommunicationSPS_h
#define CommunicationSPS_h

/**
 * @file CommunicationSPS.h
 * @author Joel Bommeli (joel.bommeli@hof-university.de)
 * 
 * @brief Funktionen zur Kommunikation mit der SPS über RS-485
 * 
 * Siehe folgende Möglichkeiten zur Einstellung in Settings.h:
 * 
 * 
 * 
 * #SPS_Connected
 * 
 * #SPS_UART_Timeout
 * 
 * #SPS_UART_RxCommandMemory
 * 
 * #SPS_UART_Frequency
 * 
 * #C_MC_DataBufferSize
 * 
 * 
 * @version 0.1
 * @date 2025-01-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include <Arduino.h>
#include <Definitions.h>

/**
 * @brief Funktionszeiger für für dynamischen Aufruf von Funktionen.
 * @return Funktion darf keinen Rückgabewert haben
 * @param uint8_t * Funktion darf Argumente in Form eines uint8_t-Zeigers haben
 */
typedef void (*command_function)(uint8_t *);

/**
 * @brief Definition von SPS-Befehlspaketen. 
 * 
 * @param cmd Wert des Befehlsbyte des zugeordneten Befehlspakets
 * @param function Auszuführende Funktion bei Empfang des entsprechenden Befehlsbytes
 */
void comSPS_addCommand(uint8_t cmd, command_function function);

/**
 * @brief Daten von SPS empfangen und ausführen, sofern definiert.
 */
void comSPS_execute();

/**
 * @brief Initialisiere UART-Interface und in comSPS_protocol() definiertes Protokoll.
 */
void comSPS_init();

/**
 * @brief Synchronisiere SPS und CarControlUnit mithilfe des Life-/Polling-Signals; blockierende Funktion.
 */
void comSPS_sync();

/**
 * @brief Sende Daten an die SPS. Wird sofort gesendet und nicht gepuffert. Für Bestätigung von empfangenenen Befehlspaketen. 
 * @warning Kann bei falscher Verwendung die Kommunikation stören.
 * 
 * @param cmd Befehl
 * @param data Daten
 */
void comSPS_send2(uint8_t cmd, uint8_t data);

/**
 * @brief Sende Daten an die SPS. Wird in den Sendepuffer geschrieben und bei Polling an die SPS gesendet.
 * 
 * @param cmd Befehl
 * @param data Daten
 */
void comSPS_writeData(uint8_t cmd, uint8_t data);

/**
 * @brief Beantwortung des Polling-Pakets der SPS. Sendet Daten aus dem Puffer an die SPS, falls vorhanden ansonsten das Life-Signal.
 * @see comSPS_writeData()
 */
void comSPS_sendDataPacket();                       
#endif