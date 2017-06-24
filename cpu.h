#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <QBitArray>

const int8_t SIGN_BIT = 0b10000000;

class CPU
{
public:
    CPU();
    struct dataRegisters {
        int8_t A;
        int8_t B;
        int8_t C;
        int8_t D;
        int8_t E;
        int8_t H;
        int8_t L;
    } registers;

    struct flagRegisters {
        bool aux;
        bool sign;
        bool zero;
        bool parity;
        bool carry;
    } flags;


   int8_t calculateParity(int8_t);
   int addBytes(int8_t, int8_t, bool, bool);

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
};

#endif // CPU_H
