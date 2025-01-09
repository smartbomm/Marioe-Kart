//Beispielfunktion, die aufgerufen soll, wenn der Befehl 7 empfangen wird
void spsCommand(uint8_t * buffer){

    //Aufruf von Daten aus dem empfangenen Befehlspaket
    uint8_t data = buffer[1];
}

//Beispielfunktion, die aufgerufen soll, wenn der Befehl 8 empfangen wird
void spsCommand2(uint8_t * buffer){

    //Aufruf von Daten aus dem empfangenen Befehlspaket
    uint8_t data = buffer[1];
}

void setup() {

    comSPS_addCommand(7, spsCommand);           //Füge die Funktion spsCommand hinzu, die ausgeführt wird, wenn der Befehl 7 empfangen wird
    comSPS_addCommand(8, spsCommand2);          //Füge die Funktion spsCommand2 hinzu, die ausgeführt wird, wenn der Befehl 8 empfangen wird
}
