#ifndef CommunicationSPS_h
#define CommunicationSPS_h

/** 
 * @page Kommunikation
 *
 * @brief Alle Funktionen zur Kommunikation mit einer Gegenstelle (SPS) über RS-485 sind in der Komponente CommunicationSPS definiert. Siehe Dokumentation von CommunicationSPS.h .
 * 
 * Die CarControlUnit ist als Slave konzipiert und wird über RS-485 angesteuert. Mithilfe der Komponente CommunicationSPS wir die Kommunikation implementiert. 
 * Damit ist es möglich beliebige Befehlspakete von einer übergeordneten Steuerung zu empfangen und auszuführen. 
 * Diese können einfach definiert werden und jeweils an eine bestimmte Funktion übergeben werden. 
 * 
 * Die Kommunikation erfolgt über RS-485 Half-Duplex. Das bedeutet, dass jeweils nur ein Teilnehmer auf einmal senden kann. Damit es nicht zu Kollisionen kommt, gibt die übergeordnete Steuerung (SPS) den Takt vor.
 * Die CarControlUnit sendet nur dann Daten, wenn die übergeordnete Steuerung dies anfordert. Dabei werden zwei Arten von Paketen unterschieden:
 *  Paket | Beschreibung | Grösse
 * -------|--------------|-------
 * Befehlspaket | Wird von der SPS an die CarControlUnit gesendet und enthält einen Befehl und Daten | 5 Bytes
 * Datenpaket | Wird von der CarControlUnit an die SPS gesendet und enthält Daten | 2 Bytes
 * 
 * @section paketaufbau Paketaufbau
 * 
 * ###Befehlspaket
 * 
 * Ein Befehlspaket hat grundsätzlich den folgenden Aufbau:
 * 
 * Byte 0 | Byte 1 | Byte 2 | Byte 3 | Byte 4
 * -------|--------|--------|--------|-------
 * Befehl | Daten  | Daten  | Daten  | Daten
 * 
 * Der Befehl gibt an, welche Funktion ausgeführt werden soll. Die Daten sind optional und können je nach Befehl unterschiedlich interpretiert werden. 
 * Es gibt ein besonderes Befehlspaket, das als Polling- und Synchronisationspaket dient. Dieses ist in der Datei Settings.h unter #C_SPS_SYNC definiert.
 * 
 * Neue Befehlspakete können mittels der Funktion `comSPS_addCommand()` angelegt werden. Dazu muss zuerst eine Funktion definiert werden, die bei dem entsprechenden Befehl ausgeführt werden soll. Danach wird diese Funktion mittels `comSPS_addCommand() dem Befehl zugeordnet.
 * Die Funktionen, die den Befehlen zugeordnet werden, müssen als Argument einen uint8_t-Zeiger haben und dürfen keinen Rückgabewert haben. Beispiel:
 * 
 * ```
 * void exampleCommand(uint8_t * packet);
 * ```
 * 
 * @note Die Anzahl der Befehle, die definiert werden können, ist in Settings.h unter #SPS_UART_RxCommandMemory festgelegt. Dieser Wert muss jedoch mindestens der höchsten Befehlsnummer entsprechen, sonst wird das Programm nicht funktionieren.
 * 
 * ### Beispiel
 * 
 * \include{lineno} comSPS_addCommand.cpp
 * 
 * 
 * 
 * 
 * ###Datenpaket
 * 
 * Ein Datenpaket ist wie folgt aufgebaut:
 * 
 * Byte 0 | Byte 1
 * -------|-------
 * Befehl | Daten
 * 
 * Zum senden von Datenpaketen gibt es zwei verschiedene Funktionen:
 * 
 * 1. `comSPS_send2(uint8_t command, uint8_t data)` - Sendet sofort bei Aufruf das Datenpaket an die übergeordnete Steuerung. Diese Funktion ist für die Bestätigung von empfangenen Befehlspaketen konzipiert.
 * 2. `comSPS_writeData(uint8_t command, uint8_t data)` - Fügt das Datenpaket in eine Warteschlange ein und sendet dann die Pakete jeweils bei Polling durch die übergeordnete Steuerung. 
 *     Die Länge der Warteschlange ist in Settings.h unter #C_MC_DataBufferSize definiert. Diese Funktion ist für das Senden von Daten an die übergeordnete Steuerung konzipiert, die gepollt werden müssen.
 * 
 * Die verschiedenen Datenpakete sind als Makros in Settings.h definiert. 
 * - `C_MC_LIFESIGNAL` - Lifesignal der CarControlUnit
 * - `C_MC_CarIN(value)` - Fahrzeug in Boxengasse eingefahren
 * - `C_MC_CarOUT(value)` - Fahrzeug aus Boxengasse ausgefahren
 * - `C_MC_CarPROGRAMMED(value)` - Fahrzeug fertig programmiert
 * - `C_MC_OK(value)` - Bestätigungspaket für empfangene Befehlspakete
 * 
 * 
 * @section erweiterung-protokoll Mehrere Bus-Teilnehmer
 * Aktuell ist das Protokoll nur für die Kommunikation zwischen einem Master und einem Slave ausgelegt. 
 * Wenn weitere Slaves mit der übergeordnete Steuerung kommunizieren sollen, muss das Protokoll um eine Adressierung erweitert werden.
 * 
 * 
 * Um die Paketlänge zu verändern, muss die Definition #SPS_UART_RxPacketLength auf die entsprechende Länge angepasst werden.
 * 
 * ### Synchronisierung mit übergeordneter Steuerung
 * - Die Definition #C_SPS_SYNC muss auf die entsprechende Länge ergänzt werden. 
 * - Die Funktion comSPS_sync() muss um eine Logik zur Erkennung der Adressierung ergänzt werden.
 * 
 * ### Empfangen von Befehlspaketen von der übergeordneten Steuerung
 * - Die Funktion comSPS_execute() muss um eine Logik zur zur Erkennung der Adressierung ergänzt werden.
 * 
 * ### Senden von Datenpaketen an die übergeordnete Steuerung
 * - Die Definition von #SPS_UART_TxPacketLength muss auf die entsprechende Länge ergänzt werden. 
 * - Die Funktion comSPS_send2() muss um eine Logik zur Adressierung ergänzt werden.
 * - Die Funktion comSPS_sendDataPacket() muss um eine Logik zur Adressierung ergänzt werden.
 * 
 * Moduldatei: CommunicationSPS.cpp
 * 
 */

/**
 * @file CommunicationSPS.h
 * @author Joel Bommeli (joel.bommeli@hof-university.de)
 * 
 * @brief Funktionen zur Kommunikation mit der SPS über RS-485
 * 
 * Siehe folgende Möglichkeiten zur Einstellung in Settings.h:
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
 * @brief Beantwortung des Polling-Pakets der SPS. Sendet Daten aus dem Puffer an die SPS, falls vorhanden, ansonsten das Life-Signal.
 * @see comSPS_writeData()
 */
void comSPS_sendDataPacket();                       
#endif