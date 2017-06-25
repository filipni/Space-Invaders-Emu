#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <QBitArray>

const uint8_t SIGN_BIT 				= 0b10000000;
const uint8_t ZERO_BIT 			= 0b01000000;
const uint8_t AUX_BIT 				= 0b00010000;
const uint8_t PARITY_BIT 			= 0b00000100;
const uint8_t CARRY_BIT 			= 0b00000001;

const uint8_t EMPTY_FLAG_REGISTER 	= 0b00000010;

class CPU
{
public:
    CPU();
    struct dataRegisters {
        int8_t A; // Accumulator
        int8_t B;
        int8_t C;
        int8_t D;
        int8_t E;
        int8_t H;
        int8_t L;
    } registers;

    /*
    struct flagRegisters {
        bool aux;
        bool sign;
        bool zero;
        bool parity;
        bool carry;
    } flags;
    */

   uint8_t conditionBits;
   void setConditionBit(uint8_t);
   void setConditionBit(uint8_t, bool);
   void clearConditionBit(uint8_t);
   void toggleConditionBit(uint8_t);
   bool testConditionBit(uint8_t);

   uint8_t calculateParity(uint8_t);
   int addBytes(int8_t, int8_t, bool, uint8_t);

   void NOP();
   void CMC();
   void STC();

   void INR(int8_t&);
   void INR_A();
   void INR_B();
   void INR_C();
   void INR_D();
   void INR_E();
   void INR_H();
   void INR_L();
   void INR_M();

   void DCR(int8_t &reg);
   void DCR_A();
   void DCR_B();
   void DCR_C();
   void DCR_D();
   void DCR_E();
   void DCR_H();
   void DCR_L();
   void DCR_M();

   void CMA();
};

#endif // CPU_H
