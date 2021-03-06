#ifndef FLAGREGISTER_H
#define FLAGREGISTER_H

#include <stdint.h>

const uint8_t SIGN_BIT 				= 0b10000000;
const uint8_t ZERO_BIT 			    = 0b01000000;
const uint8_t AUX_BIT 				= 0b00010000;
const uint8_t PARITY_BIT 			= 0b00000100;
const uint8_t CARRY_BIT 			= 0b00000001;

const uint8_t EMPTY_FLAG_REGISTER 	= 0b00000010;

class FlagRegister
{
public:
    FlagRegister();
    FlagRegister(uint8_t);

    uint8_t getRegister();
    void setBits(uint8_t bitmask);
    void setBits(uint8_t bitmask, bool val);
    void clearBits(uint8_t bitmask);
    void toggleBits(uint8_t bitmask);
    bool testBits(uint8_t bitmask);

    uint8_t calculateParity(uint8_t);
    void calculateEvenParityBit(uint8_t);
    void calculateSignBit(uint8_t);
    void calculateZeroBit(uint8_t);
    void calculateZeroSignParityBits(uint8_t);

private:
    uint8_t conditionBits;
};

#endif // FLAGREGISTER_H
