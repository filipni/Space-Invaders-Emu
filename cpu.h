#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <cstring>
#include <QDebug>
#include <QKeyEvent>
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

const int COIN = 1;
const int P2_START = 1 << 1;
const int P1_START = 1 << 2;
const int P1_SHOOT = 1 << 4;
const int P1_LEFT = 1 << 5;
const int P1_RIGHT = 1 << 6;

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

   bool interruptsEnabled = false;

   uint8_t input1 = 0;
   uint8_t input2 = 0;
   uint8_t input3 = 0;

   uint8_t output2 = 0;
   uint8_t output3 = 0;
   uint8_t output4 = 0;
   uint8_t output5 = 0;
   uint8_t output6 = 0;

   uint16_t shiftRegister = 0;

   uint8_t memory[MEMORY_SIZE];

   uint8_t getHighBits(uint16_t);
   uint8_t getLowBits(uint16_t);
   uint16_t create16BitReg(uint8_t, uint8_t);

   int addBytes(uint8_t, uint8_t, bool, FlagRegister);
   uint8_t getBit(uint8_t, uint8_t);

   void shiftRegisterOp();

   int CMC();
   int STC();

   int INR(uint8_t&);
   int INR_A();
   int INR_B();
   int INR_C();
   int INR_D();
   int INR_E();
   int INR_H();
   int INR_L();
   int INR_M();

   int DCR(uint8_t &reg);
   int DCR_A();
   int DCR_B();
   int DCR_C();
   int DCR_D();
   int DCR_E();
   int DCR_H();
   int DCR_L();
   int DCR_M();

   int CMA();
   int DAA();
   int NOP();

   int MOV_B_B();
   int MOV_B_C();
   int MOV_B_D();
   int MOV_B_E();
   int MOV_B_H();
   int MOV_B_L();
   int MOV_B_M();
   int MOV_B_A();

   int MOV_C_B();
   int MOV_C_C();
   int MOV_C_D();
   int MOV_C_E();
   int MOV_C_H();
   int MOV_C_L();
   int MOV_C_M();
   int MOV_C_A();

   int MOV_D_B();
   int MOV_D_C();
   int MOV_D_D();
   int MOV_D_E();
   int MOV_D_H();
   int MOV_D_L();
   int MOV_D_M();
   int MOV_D_A();

   int MOV_E_B();
   int MOV_E_C();
   int MOV_E_D();
   int MOV_E_E();
   int MOV_E_H();
   int MOV_E_L();
   int MOV_E_M();
   int MOV_E_A();

   int MOV_H_B();
   int MOV_H_C();
   int MOV_H_D();
   int MOV_H_E();
   int MOV_H_H();
   int MOV_H_L();
   int MOV_H_M();
   int MOV_H_A();

   int MOV_L_B();
   int MOV_L_C();
   int MOV_L_D();
   int MOV_L_E();
   int MOV_L_H();
   int MOV_L_L();
   int MOV_L_M();
   int MOV_L_A();

   int MOV_M_B();
   int MOV_M_C();
   int MOV_M_D();
   int MOV_M_E();
   int MOV_M_H();
   int MOV_M_L();
   int MOV_M_A();

   int MOV_A_B();
   int MOV_A_C();
   int MOV_A_D();
   int MOV_A_E();
   int MOV_A_H();
   int MOV_A_L();
   int MOV_A_M();
   int MOV_A_A();

   int STAX_B();
   int STAX_D();

   int LDAX_B();
   int LDAX_D();

   int ADD(uint8_t);
   int ADD_B();
   int ADD_C();
   int ADD_D();
   int ADD_E();
   int ADD_H();
   int ADD_L();
   int ADD_M();
   int ADD_A();

   int ADC(uint8_t);
   int ADC_B();
   int ADC_C();
   int ADC_D();
   int ADC_E();
   int ADC_H();
   int ADC_L();
   int ADC_M();
   int ADC_A();

   int SUB(uint8_t);
   int SUB_B();
   int SUB_C();
   int SUB_D();
   int SUB_E();
   int SUB_H();
   int SUB_L();
   int SUB_M();
   int SUB_A();

   int SBB(uint8_t);
   int SBB_B();
   int SBB_C();
   int SBB_D();
   int SBB_E();
   int SBB_H();
   int SBB_L();
   int SBB_M();
   int SBB_A();

   int ANA(uint8_t);
   int ANA_B();
   int ANA_C();
   int ANA_D();
   int ANA_E();
   int ANA_H();
   int ANA_L();
   int ANA_M();
   int ANA_A();

   int XRA(int8_t);
   int XRA_B();
   int XRA_C();
   int XRA_D();
   int XRA_E();
   int XRA_H();
   int XRA_L();
   int XRA_M();
   int XRA_A();

   int ORA(uint8_t);
   int ORA_B();
   int ORA_C();
   int ORA_D();
   int ORA_E();
   int ORA_H();
   int ORA_L();
   int ORA_M();
   int ORA_A();

   int CMP(uint8_t);
   int CMP_B();
   int CMP_C();
   int CMP_D();
   int CMP_E();
   int CMP_H();
   int CMP_L();
   int CMP_M();
   int CMP_A();

   int RLC();
   int RRC();
   int RAL();
   int RAR();

   int PUSH_B();
   int PUSH_D();
   int PUSH_H();
   int PUSH_PSW();

   int POP_B();
   int POP_D();
   int POP_H();
   int POP_PSW();

   int JMP();

   int conditionalJump(bool);
   int JC();
   int JNC();
   int JZ();
   int JNZ();
   int JM();
   int JP();
   int JPE();
   int JPO();

   int LXI_B();
   int LXI_D();
   int LXI_H();
   int LXI_SP();

   int MVI_B();
   int MVI_C();
   int MVI_D();
   int MVI_E();
   int MVI_H();
   int MVI_L();
   int MVI_M();
   int MVI_A();

   int CALL();

   double conditionalCall(bool);
   double CC();
   double CNC();
   double CZ();
   double CNZ();
   double CM();
   double CP();
   double CPE();
   double CPO();

   int RET();

   double conditionalReturn(bool);
   double RC();
   double RNC();
   double RZ();
   double RNZ();
   double RM();
   double RP();
   double RPE();
   double RPO();

   int LDA();
   int STA();
   int SHLD();
   int LHLD();

   int INX_B();
   int INX_D();
   int INX_H();
   int INX_SP();

   int DCX_B();
   int DCX_D();
   int DCX_H();
   int DCX_SP();

   int EI();
   int DI();

   int RST(uint8_t);
   int RST_0();
   int RST_1();
   int RST_2();
   int RST_3();
   int RST_4();
   int RST_5();
   int RST_6();
   int RST_7();

   int IN();
   int OUT();

   int DAD(uint8_t, uint8_t);
   int DAD_B();
   int DAD_D();
   int DAD_H();
   int DAD_SP();

   int ADI();
   int SUI();
   int ANI();
   int ORI();
   int ACI();
   int SBI();
   int XRI();
   int CPI();

   int PCHL();
   int XCHG();
   int XTHL();
   int SPHL();
};

#endif // CPU_H
