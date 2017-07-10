#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <cstring>
#include "flagregister.h"

const uint8_t HIGH_ORDER_BIT = 0x80;
const uint8_t LOW_ORDER_BIT = 0x01;

const int MEMORY_SIZE = 0x4000;

const int ROM_START = 0x00;
const int ROM_SIZE = 0x2000;

const int WORK_RAM_START = 0x2000;
const int WORK_RAM_SIZE = 0x400;

const int VIDEO_RAM_START = 0x2400;
const int VIDEO_RAM_SIZE = 0x1C00;

class CPU
{
public:
    FlagRegister conditionBits;

    CPU();
    struct dataRegisters {
        uint8_t A; // Accumulator
        uint8_t B;
        uint8_t C;
        uint8_t D;
        uint8_t E;
        uint8_t H;
        uint8_t L;
        uint16_t PC;
        uint16_t SP;
    } registers;

   uint8_t memory[MEMORY_SIZE];

   uint8_t getHighBits(uint16_t);
   uint8_t getLowBits(uint16_t);
   uint16_t create16BitReg(uint8_t, uint8_t);

   int addBytes(uint8_t, uint8_t, bool, FlagRegister);
   uint8_t getBit(uint8_t, uint8_t);

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

   void DCR(uint8_t &reg);
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

   void STAX_B();
   void STAX_D();

   void LDAX_B();
   void LDAX_D();

   void ADD(uint8_t);
   void ADD_B();
   void ADD_C();
   void ADD_D();
   void ADD_E();
   void ADD_H();
   void ADD_L();
   void ADD_M();
   void ADD_A();

   void ADC(uint8_t);
   void ADC_B();
   void ADC_C();
   void ADC_D();
   void ADC_E();
   void ADC_H();
   void ADC_L();
   void ADC_M();
   void ADC_A();

   void SUB(uint8_t);
   void SUB_B();
   void SUB_C();
   void SUB_D();
   void SUB_E();
   void SUB_H();
   void SUB_L();
   void SUB_M();
   void SUB_A();

   void SBB(uint8_t);
   void SBB_B();
   void SBB_C();
   void SBB_D();
   void SBB_E();
   void SBB_H();
   void SBB_L();
   void SBB_M();
   void SBB_A();

   void ANA(uint8_t);
   void ANA_B();
   void ANA_C();
   void ANA_D();
   void ANA_E();
   void ANA_H();
   void ANA_L();
   void ANA_M();
   void ANA_A();

   void XRA(int8_t);
   void XRA_B();
   void XRA_C();
   void XRA_D();
   void XRA_E();
   void XRA_H();
   void XRA_L();
   void XRA_M();
   void XRA_A();

   void ORA(uint8_t);
   void ORA_B();
   void ORA_C();
   void ORA_D();
   void ORA_E();
   void ORA_H();
   void ORA_L();
   void ORA_M();
   void ORA_A();

   void CMP(uint8_t);
   void CMP_B();
   void CMP_C();
   void CMP_D();
   void CMP_E();
   void CMP_H();
   void CMP_L();
   void CMP_M();
   void CMP_A();

   void RLC();
   void RRC();
   void RAL();
   void RAR();

   void PUSH_B();
   void PUSH_D();
   void PUSH_H();
   void PUSH_PSW();

   void POP_B();
   void POP_D();
   void POP_H();
   void POP_PSW();

   void JMP();

   void LXI_B();
   void LXI_D();
   void LXI_H();
   void LXI_SP();

   void MVI_B();
   void MVI_C();
   void MVI_D();
   void MVI_E();
   void MVI_H();
   void MVI_L();
   void MVI_M();
   void MVI_A();

   void CALL();

   void conditionalCall(bool);
   void CC();
   void CNC();
   void CZ();
   void CNZ();
   void CM();
   void CP();
   void CPE();
   void CPO();

   void RET();

   void conditionalReturn(bool);
   void RC();
   void RNC();
   void RZ();
   void RNZ();
   void RM();
   void RP();
   void RPE();
   void RPO();
};

#endif // CPU_H
