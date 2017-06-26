#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <QBitArray>
#include <flagregister.h>

class CPU
{
public:
    FlagRegister conditionBits;

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
