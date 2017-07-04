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

uint8_t FlagRegister::calculateParity(uint8_t reg) // Register needs to be converted to unsigned for method to work
{
   uint8_t count = 0;
   while (reg)
   {
       if ((reg & 1) == 1)
           ++count;
       reg >>= 1;
   }
   return count;
}

void FlagRegister::calculateEvenParityBit(uint8_t reg)
{
    setBits(PARITY_BIT, calculateParity(reg) % 2 == 0);
}

void FlagRegister::calculateSignBit(uint8_t reg)
{
    setBits(SIGN_BIT, (reg & SIGN_BIT) != 0);
}

void FlagRegister::calculateZeroBit(uint8_t reg)
{
    setBits(ZERO_BIT, reg == 0);
}

void FlagRegister::calculateZeroSignParityBits(uint8_t reg)
{
    calculateZeroBit(reg);
    calculateSignBit(reg);
    calculateEvenParityBit(reg);
}
