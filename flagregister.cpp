#include "flagregister.h"

FlagRegister::FlagRegister()
{
    conditionBits = EMPTY_FLAG_REGISTER;
}

FlagRegister::FlagRegister(uint8_t bitmask)
{
    conditionBits = EMPTY_FLAG_REGISTER | bitmask;
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

bool FlagRegister::testBits(uint8_t bitmask)
{
    uint8_t result = conditionBits & bitmask;
    return result == bitmask;
}
