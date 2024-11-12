#include <CommunicationSPS/Command.h>


Command::Command(uint8_t packet_length){
    word_size = packet_length;

}

inline void Command::add(uint8_t cmd, command_function function){
    commands[cmd] = function;
}
void Command::execute(uint8_t cmd, byte * buffer){
    commands[cmd](buffer, word_size);
}