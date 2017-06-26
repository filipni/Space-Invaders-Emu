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

    void setBits(uint8_t bitmask);
    void setBits(uint8_t bitmask, bool val);
    void clearBits(uint8_t bitmask);
    void toggleBits(uint8_t bitmask);
    bool testBit(uint8_t bitmask);

private:
    uint8_t conditionBits;
};

#endif // FLAGREGISTER_H
