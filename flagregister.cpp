#include "flagregister.h"

FlagRegister::FlagRegister()
{
    conditionBits = EMPTY_FLAG_REGISTER;
}

FlagRegister::FlagRegister(uint8_t bitmask)
{
    conditionBits = EMPTY_FLAG_REGISTER;
    setBits(bitmask);
}

void FlagRegister::setBits(uint8_t bitmask)
{
    conditionBits |= bitmask;
}

void FlagRegister::setBits(uint8_t bitmask, bool val)
{
    val ? setBits(bitmask) : clearBits(bitmask);
}

void FlagRegister::clearBits(uint8_t bitmask)
{
    conditionBits &= (bitmask ^ 0xFF);
}

void FlagRegister::toggleBits(uint8_t bitmask)
{
    conditionBits ^= bitmask;
}

bool FlagRegister::testBit(uint8_t bitmask)
{
    return conditionBits & bitmask;
}
