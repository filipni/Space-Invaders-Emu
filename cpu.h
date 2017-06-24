#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <QBitArray>

class CPU
{
public:
    CPU();
    struct dataRegisters {
        uint8_t A;
        uint8_t B;
        uint8_t C;
        uint8_t D;
        uint8_t E;
        uint8_t H;
        uint8_t L;
    } registers;

    struct flagRegisters {
        bool aux;
        bool sign;
        bool zero;
        bool parity;
        bool carry;
    } flags;


   uint8_t calculateParity(uint8_t);
   int addBytes(uint8_t, uint8_t, bool, bool);

   void NOP();
   void CMC();
   void STC();

   void INR(uint8_t&);
   void INR_A();
   void INR_B();
   void INR_C();
   void INR_D();
   void INR_E();
   void INR_H();
   void INR_L();
   void INR_M();
};

#endif // CPU_H
