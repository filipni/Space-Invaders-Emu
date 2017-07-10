#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>
#include "cpu.h"

#define ROM_FILE_PATH "/home/filip/Dropbox/SI2/Disassembler/invaders.rom"

class Emulator
{
public:
    Emulator();
    void run();
    int decode(uint8_t);
    CPU cpu;

};

#endif // EMULATOR_H
