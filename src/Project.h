/** @mainpage CarControlUnit 
 *  
 *  @section sec1 Softwaredokumentation
 * 
 * Abkürzungen:
 * 
 * 
 * CCU - CarControlUnit  
 * CDU - CarDetectionUnit 
 *  
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
 * ## Aufbau der Pakete
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
 * 
 * 
 * 
 *  \include{lineno} example.cpp
 * \
 * This is an example of how to use the Example_Test class.
 * More details about this example.
 * 
 * @page Fahrzeugerkennung
 * 
 * @page Fahrzeugsteuerung
 */
