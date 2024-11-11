#ifndef ComSPS_Protocol
#define ComSPS_Protocol
#include <Arduino.h>

typedef void (*void_function)(void);

class SPSCommand {
    public:
        uint8_t command;
        void execute();
};






#endif