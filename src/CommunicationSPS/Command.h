#ifndef Command_h
#define Command_h
#include <Arduino.h>
#include <Settings.h>

typedef void (*command_function)(uint8_t *, uint8_t); // function (uint8_t* pointer to buffer, uint8_t size of buffer)

class Command
{
private:
    command_function commands[SPS_UART_CommandMemory];
    uint8_t word_size;

public:
    Command(uint8_t packet_length);
    void add(uint8_t cmd, command_function function);
    void execute(uint8_t cmd, byte *buffer);
};

#endif