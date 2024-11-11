#ifndef CommunicationSPS
#define CommunicationSPS
#include <Arduino.h>

void initializeCom();
uint8_t command(byte * packet);
void answer();


#endif