/** @mainpage CarControlUnit 
 *  
 * Dieses Projekt entstand im Rahmen einer Projektarbeit des Moduls "Projekt Automatisierung" an der Hochschule für angewandte Wissenschaften Hof.
 * Das Ziel des Gesamtprojekts war es, eine automatisierte Boxengasse zu entwickeln, die einen automatisierten Fahrzeugwechsel 
 * sowie die Ein- und Auslagerung von Carrera Digital 124-Fahrzeugen in ein Lager ermöglicht.
 * 
 * Das Modul CarControlUnit ermöglicht die Ansteuerung der Programmierschiene, 
 * über welche die Ein- und Ausfahrt der Fahrzeuge in die Boxengasse erfolgt.
 * 
 * Die Hardware besteht aus:
 * - Eine CarControlUnit, auf welcher ein Mikrocontroller (ESP32-S3) sowie der Bahntreiber installiert ist.
 * - Bis zu drei CarDetectionUnits, mit einer Aufbereitungsschaltung für die zur Fahrzeugerkennung notwendigen IR-LEDs.
 * 
 * Es werden im Projekt folgende Abkürzungen verwendet:
 * 
 * - CCU - CarControlUnit
 * - CDU - CarDetectionUnit
 * - SPS - Übergeordnete Steuerung
 * 
 * @section settings_defs Einstellungen/Definitionen
 * 
 * Damit wichtige Paramater und Einstellungen zentral verwaltet und abgerufen werden können, wurden die zwei Dateien Settings.h und Definitions.h erstellt.
 * Die zwei Dateien unterscheiden sich dabei wie folgt:
 * 
 * ## Definitionen
 * 
 * @warning Wird an dieser Datei etwas geändert, funktioniert möglicherweise das Programm nicht mehr.
 * Enthält Definitionen, die die Funktion des Programms beeinflussen.
 * 
 * - Pinbelegungen
 * - UART-Interfaces
 * - Makros
 * 
 * @ref Definitions.h "Zur Datei"
 * ## Einstellungen
 * 
 * Enthält Einstellungen und Parameter, die die Funktion des Programms nicht beeinflussen. z.B. Geschwindigkeiten, Zeiten
 * 
 * @ref Settings.h "Zur Datei"
 * @page Fahrzeugsteuerung
 */
