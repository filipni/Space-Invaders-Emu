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
   int addBytes(int8_t, int8_t, bool, FlagRegister);

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
   void DAA();
   void NOP();

   void MOV_B_B();
   void MOV_B_C();
   void MOV_B_D();
   void MOV_B_E();
   void MOV_B_H();
   void MOV_B_L();
   void MOV_B_A();

   void MOV_C_B();
   void MOV_C_C();
   void MOV_C_D();
   void MOV_C_E();
   void MOV_C_H();
   void MOV_C_L();
   void MOV_C_M();
   void MOV_C_A();

   void MOV_D_B();
   void MOV_D_C();
   void MOV_D_D();
   void MOV_D_E();
   void MOV_D_H();
   void MOV_D_L();
   void MOV_D_M();
   void MOV_D_A();

   void MOV_E_B();
   void MOV_E_C();
   void MOV_E_D();
   void MOV_E_E();
   void MOV_E_H();
   void MOV_E_L();
   void MOV_E_M();
   void MOV_E_A();

   void MOV_H_B();
   void MOV_H_C();
   void MOV_H_D();
   void MOV_H_E();
   void MOV_H_H();
   void MOV_H_L();
   void MOV_H_M();
   void MOV_H_A();

   void MOV_L_B();
   void MOV_L_C();
   void MOV_L_D();
   void MOV_L_E();
   void MOV_L_H();
   void MOV_L_L();
   void MOV_L_M();
   void MOV_L_A();

   void MOV_A_B();
   void MOV_A_C();
   void MOV_A_D();
   void MOV_A_E();
   void MOV_A_H();
   void MOV_A_L();
   void MOV_A_M();
   void MOV_A_A();
};

#endif // CPU_H
