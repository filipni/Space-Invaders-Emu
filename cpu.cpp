#include "cpu.h"
#include <cstring>
#include <QtGlobal>
CPU::CPU() : memory()
{
    memset(&registers, 0, sizeof(registers));
    interruptsEnabled = false;
}

uint8_t CPU::getHighBits(uint16_t reg)
{
   return (reg & 0xFF00) >> 8;
}

uint8_t CPU::getLowBits(uint16_t reg)
{
   return reg & 0x00FF;
}

uint16_t CPU::create16BitReg(uint8_t lowBits, uint8_t highBits)
{
    return (highBits << 8) + lowBits;
}

int CPU::addBytes(uint8_t byte1, uint8_t byte2, bool carryIn, FlagRegister flagsToCalc)
{
    uint8_t carry = carryIn ? conditionBits.testBits(CARRY_BIT) : 0;

    int8_t sum = 0;
    for (int i = 0; i <= 7; ++i)
    {
        int8_t term1 = byte1 & 1;
        int8_t term2 = byte2 & 1;

        sum += (term1 ^ term2 ^ carry) << i;
        carry = term1 + term2 + carry > 1;

        if (i == 3 && flagsToCalc.testBits(AUX_BIT))
            conditionBits.setBits(AUX_BIT, carry); // Carry from lower to higher nibble

        byte1 >>= 1;
        byte2 >>= 1;
    }

    Q_ASSERT(carry == 1 || carry == 0);

    // Set the rest of the flags
    if (flagsToCalc.testBits(CARRY_BIT)) conditionBits.setBits(CARRY_BIT, carry);
    if (flagsToCalc.testBits(ZERO_BIT)) conditionBits.calculateZeroBit(sum);
    if (flagsToCalc.testBits(SIGN_BIT)) conditionBits.calculateSignBit(sum);
    if (flagsToCalc.testBits(PARITY_BIT)) conditionBits.calculateEvenParityBit(sum);

    return sum;
}

void CPU::NOP() {}

void CPU::CMC()
{
    conditionBits.toggleBits(CARRY_BIT);
}

void CPU::STC()
{
    conditionBits.setBits(CARRY_BIT);
}

void CPU::INR(uint8_t &reg)
{
    FlagRegister flagsToCalc(SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    reg = addBytes(reg, 1, false, flagsToCalc);
}

void CPU::INR_A() { INR(registers.A); }
void CPU::INR_B() { INR(registers.B); }
void CPU::INR_C() { INR(registers.C); }
void CPU::INR_D() { INR(registers.D); }
void CPU::INR_E() { INR(registers.E); }
void CPU::INR_H() { INR(registers.H); }
void CPU::INR_L() { INR(registers.L); }
//void CPU::INR_M() { INR(registers.M); }

void CPU::DCR(uint8_t &reg)
{
    reg = addBytes(reg, -1, false, SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
}

void CPU::DCR_A() { DCR(registers.A); }
void CPU::DCR_B() { DCR(registers.B); }
void CPU::DCR_C() { DCR(registers.C); }
void CPU::DCR_D() { DCR(registers.D); }
void CPU::DCR_E() { DCR(registers.E); }
void CPU::DCR_H() { DCR(registers.H); }
void CPU::DCR_L() { DCR(registers.L); }
//void CPU::DCR_M() { DCR(registers.M); }

void CPU::CMA()
{
    registers.A = registers.A ^ 0xFF;
}

void CPU::DAA()
{
    int8_t lowerNibble = registers.A & 0x0F;
    if (lowerNibble > 9 || conditionBits.testBits(AUX_BIT))
    {
        FlagRegister flagsToCalc(SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
        registers.A = addBytes(registers.A, 6, false, flagsToCalc);
    }

    int8_t higherNibble = (registers.A & 0xF0) >> 4;
    if (higherNibble > 9 || conditionBits.testBits(CARRY_BIT))
    {
        bool oldCarry = conditionBits.testBits(CARRY_BIT);

        FlagRegister flagsToCalc(SIGN_BIT | ZERO_BIT | PARITY_BIT | CARRY_BIT);
        higherNibble = addBytes(higherNibble, 6, false, flagsToCalc);

        // Change the higher nibble in register A to the newly calculated value
        registers.A &= 0x0F;
        registers.A += higherNibble << 4;

        if (!conditionBits.testBits(CARRY_BIT)) // If no carry out occurs, reset carry bit to old value
            conditionBits.setBits(CARRY_BIT, oldCarry);
    }
}

void CPU::MOV_B_B()
{
    registers.B = registers.B;
}

void CPU::MOV_B_C()
{
    registers.B = registers.C;
}


void CPU::MOV_B_D()
{
    registers.B = registers.D;
}

void CPU::MOV_B_E()
{
    registers.B = registers.E;
}

void CPU::MOV_B_H()
{
    registers.B = registers.H;
}

void CPU::MOV_B_L()
{
    registers.B = registers.L;
}

void CPU::MOV_B_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.B = memory[loadAddr];
}

void CPU::MOV_B_A()
{
    registers.B = registers.A;
}

void CPU::MOV_C_B()
{
    registers.C = registers.B;
}

void CPU::MOV_C_C()
{
    registers.C = registers.C;
}

void CPU::MOV_C_D()
{
    registers.C = registers.D;
}

void CPU::MOV_C_E()
{
    registers.C = registers.E;
}

void CPU::MOV_C_H()
{
    registers.C = registers.H;
}

void CPU::MOV_C_L()
{
    registers.C = registers.L;
}

void CPU::MOV_C_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.C = memory[loadAddr];
}

void CPU::MOV_C_A()
{
    registers.C = registers.A;
}

void CPU::MOV_D_B()
{
    registers.D = registers.B;
}

void CPU::MOV_D_C()
{
    registers.D = registers.C;
}

void CPU::MOV_D_D()
{
    registers.D = registers.D;
}

void CPU::MOV_D_E()
{
    registers.D = registers.E;
}

void CPU::MOV_D_H()
{
    registers.D = registers.H;
}

void CPU::MOV_D_L()
{
    registers.D = registers.L;
}

void CPU::MOV_D_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.D = memory[loadAddr];
}

void CPU::MOV_D_A()
{
    registers.D = registers.A;
}

void CPU::MOV_E_B()
{
    registers.E = registers.B;
}

void CPU::MOV_E_C()
{
    registers.E = registers.C;
}

void CPU::MOV_E_D()
{
    registers.E = registers.D;
}

void CPU::MOV_E_E()
{
    registers.E = registers.E;
}

void CPU::MOV_E_H()
{
    registers.E = registers.H;
}

void CPU::MOV_E_L()
{
    registers.E = registers.L;
}

void CPU::MOV_E_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.E = memory[loadAddr];
}

void CPU::MOV_E_A()
{
    registers.E = registers.A;
}

void CPU::MOV_H_B()
{
    registers.H = registers.B;
}

void CPU::MOV_H_C()
{
    registers.H = registers.C;
}

void CPU::MOV_H_D()
{
    registers.H = registers.D;
}

void CPU::MOV_H_E()
{
    registers.H = registers.E;
}

void CPU::MOV_H_H()
{
    registers.H = registers.H;
}

void CPU::MOV_H_L()
{
    registers.H = registers.L;
}

void CPU::MOV_H_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.H = memory[loadAddr];
}

void CPU::MOV_H_A()
{
    registers.H = registers.A;
}

void CPU::MOV_L_B()
{
    registers.L = registers.B;
}

void CPU::MOV_L_C()
{
    registers.L = registers.C;
}

void CPU::MOV_L_D()
{
    registers.L = registers.D;
}

void CPU::MOV_L_E()
{
    registers.L = registers.E;
}

void CPU::MOV_L_H()
{
    registers.L = registers.H;
}

void CPU::MOV_L_L()
{
    registers.L = registers.L;
}

void CPU::MOV_L_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.L = memory[loadAddr];
}

void CPU::MOV_L_A()
{
    registers.L = registers.A;
}

void CPU::MOV_M_B()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.B;
}

void CPU::MOV_M_C()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.C;
}

void CPU::MOV_M_D()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.D;
}

void CPU::MOV_M_E()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.E;
}

void CPU::MOV_M_H()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.H;
}

void CPU::MOV_M_L()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.L;
}

void CPU::MOV_M_A()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.A;
}

void CPU::MOV_A_B()
{
    registers.A = registers.B;
}

void CPU::MOV_A_C()
{
    registers.A = registers.C;
}

void CPU::MOV_A_D()
{
    registers.A = registers.D;
}

void CPU::MOV_A_E()
{
    registers.A = registers.E;
}

void CPU::MOV_A_H()
{
    registers.A = registers.H;
}

void CPU::MOV_A_L()
{
    registers.A = registers.L;
}

void CPU::MOV_A_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.A = memory[loadAddr];
}

void CPU::MOV_A_A()
{
    registers.A = registers.A;
}

/*
void CPU::STAX_B()
{

}

void CPU::STAX_D()
{

}
*/

/*
void CPU::LDAX_B()
{

}

void CPU::LDAX_D()
{

}
*/

void CPU::ADD(uint8_t operand)
{
    FlagRegister flagsToCalc = CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT;
    registers.A = addBytes(registers.A, operand, false, flagsToCalc);
}

void CPU::ADD_B() { ADD(registers.B); }
void CPU::ADD_C() { ADD(registers.C); }
void CPU::ADD_D() { ADD(registers.D); }
void CPU::ADD_E() { ADD(registers.E); }
void CPU::ADD_H() { ADD(registers.H); }
void CPU::ADD_L() { ADD(registers.L); }
// void CPU::ADD_M()
void CPU::ADD_A() { ADD(registers.L); }

void CPU::ADC(uint8_t operand)
{
    FlagRegister flagsToCalc = CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT;
    registers.A = addBytes(registers.A, operand, true, flagsToCalc);
}

void CPU::ADC_B() { ADC(registers.B); }
void CPU::ADC_C() { ADC(registers.C); }
void CPU::ADC_D() { ADC(registers.D); }
void CPU::ADC_E() { ADC(registers.E); }
void CPU::ADC_H() { ADC(registers.H); }
void CPU::ADC_L() { ADC(registers.L); }
// void CPU::ADC_M() {}
void CPU::ADC_A() { ADC(registers.A); }

void CPU::SUB(uint8_t operand)
{
    FlagRegister flagsToCalc = CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT;
    int8_t operand2Cmp = (operand ^ 0xFF) + 1;
    registers.A = addBytes(registers.A, operand2Cmp, false, flagsToCalc);
    conditionBits.setBits(CARRY_BIT, !conditionBits.testBits(CARRY_BIT)); // Since this is a substraction, we invert the carry
}

void CPU::SUB_B() { SUB(registers.B); }
void CPU::SUB_C() { SUB(registers.C); }
void CPU::SUB_D() { SUB(registers.D); }
void CPU::SUB_E() { SUB(registers.E); }
void CPU::SUB_H() { SUB(registers.H); }
void CPU::SUB_L() { SUB(registers.L); }
// void CPU::SUB_M() {}
void CPU::SUB_A() { SUB(registers.A); }

void CPU::SBB(uint8_t operand)
{
    FlagRegister flagsToCalc = CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT;
    operand = operand + conditionBits.testBits(CARRY_BIT);
    int8_t operand2Cmp = (operand ^ 0xFF) + 1;
    registers.A = addBytes(registers.A, operand2Cmp, false, flagsToCalc);
    conditionBits.setBits(CARRY_BIT, !conditionBits.testBits(CARRY_BIT)); // Since this is a substraction, we invert the carry
}

void CPU::SBB_B() { SBB(registers.B); }
void CPU::SBB_C() { SBB(registers.C); }
void CPU::SBB_D() { SBB(registers.D); }
void CPU::SBB_E() { SBB(registers.E); }
void CPU::SBB_H() { SBB(registers.H); }
void CPU::SBB_L() { SBB(registers.L); }
//void CPU::SBB_M() {}
void CPU::SBB_A() { SBB(registers.A); }

void CPU::ANA(uint8_t operand)
{
    registers.A &= operand;
    conditionBits.setBits(CARRY_BIT, false);
    conditionBits.calculateZeroSignParityBits(registers.A);
}

void CPU::ANA_B() { ANA(registers.B); }
void CPU::ANA_C() { ANA(registers.C); }
void CPU::ANA_D() { ANA(registers.D); }
void CPU::ANA_E() { ANA(registers.E); }
void CPU::ANA_H() { ANA(registers.H); }
void CPU::ANA_L() { ANA(registers.L); }
// void CPU::ANA_M() { ANA(); }
void CPU::ANA_A() { ANA(registers.A); }

void CPU::XRA(int8_t operand)
{
    registers.A ^= operand;
    conditionBits.setBits(CARRY_BIT, false);
    conditionBits.calculateZeroSignParityBits(registers.A);
}

void CPU::XRA_B() { XRA(registers.B); }
void CPU::XRA_C() { XRA(registers.C); }
void CPU::XRA_D() { XRA(registers.D); }
void CPU::XRA_E() { XRA(registers.E); }
void CPU::XRA_H() { XRA(registers.H); }
void CPU::XRA_L() { XRA(registers.L); }
// void CPU::XRA_M() { XRA(); }
void CPU::XRA_A() { XRA(registers.A); }

void CPU::ORA(uint8_t operand)
{
    registers.A |= operand;
    conditionBits.setBits(CARRY_BIT, false);
    conditionBits.calculateZeroSignParityBits(registers.A);
}

void CPU::ORA_B() { ORA(registers.B); }
void CPU::ORA_C() { ORA(registers.C); }
void CPU::ORA_D() { ORA(registers.D); }
void CPU::ORA_E() { ORA(registers.E); }
void CPU::ORA_H() { ORA(registers.H); }
void CPU::ORA_L() { ORA(registers.L); }
// void CPU::ORA_M() { ORA(); }
void CPU::ORA_A() { ORA(registers.A); }

void CPU::CMP(uint8_t reg)
{
    FlagRegister flagsToCalc(SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    addBytes(registers.A, -reg, false, flagsToCalc);
}

void CPU::CMP_B() { CMP(registers.B); }
void CPU::CMP_C() { CMP(registers.C); }
void CPU::CMP_D() { CMP(registers.D); }
void CPU::CMP_E() { CMP(registers.E); }
void CPU::CMP_H() { CMP(registers.H); }
void CPU::CMP_L() { CMP(registers.L); }
// void CPU::CMP_M() { CMP(); }
void CPU::CMP_A() { CMP(registers.A); }

void CPU::RLC()
{
    uint8_t carry = (registers.A & HIGH_ORDER_BIT) >> 7;
    conditionBits.setBits(CARRY_BIT, carry);
    registers.A <<= 1;
    registers.A |= carry;
}

void CPU::RRC()
{
    uint8_t carry = registers.A & LOW_ORDER_BIT;
    conditionBits.setBits(CARRY_BIT, carry);
    registers.A >>= 1;
    registers.A = registers.A | (carry << 7);
}

void CPU::RAL()
{
    uint8_t newCarry = (registers.A & HIGH_ORDER_BIT) >> 7;
    uint8_t oldCarry = conditionBits.testBits(CARRY_BIT);
    conditionBits.setBits(CARRY_BIT, newCarry);
    registers.A <<= 1;
    registers.A |= oldCarry;
}

void CPU::RAR()
{
    uint8_t newCarry = registers.A & LOW_ORDER_BIT;
    uint8_t oldCarry = conditionBits.testBits(CARRY_BIT);
    conditionBits.setBits(CARRY_BIT, newCarry);
    registers.A >>= 1;
    registers.A = registers.A | (oldCarry << 7);
}

void CPU::PUSH_B()
{
    memory[registers.SP-1] = registers.B;
    memory[registers.SP-2] = registers.C;
    registers.SP -= 2;
}

void CPU::PUSH_D()
{
    memory[registers.SP-1] = registers.D;
    memory[registers.SP-2] = registers.E;
    registers.SP -= 2;
}

void CPU::PUSH_H()
{
    memory[registers.SP-1] = registers.H;
    memory[registers.SP-2] = registers.L;
    registers.SP -= 2;
}

void CPU::PUSH_PSW()
{
    memory[registers.SP-1] = registers.A;
    memory[registers.SP-2] = conditionBits.getRegister();
    registers.SP -= 2;
}

void CPU::POP_B()
{
    registers.C = memory[registers.SP];
    registers.B = memory[registers.SP+1];
    registers.SP += 2;
}

void CPU::POP_D()
{
    registers.E = memory[registers.SP];
    registers.D = memory[registers.SP+1];
    registers.SP += 2;
}

void CPU::POP_H()
{
    registers.L = memory[registers.SP];
    registers.H = memory[registers.SP+1];
    registers.SP += 2;
}

void CPU::POP_PSW()
{
    conditionBits = FlagRegister(memory[registers.SP]);
    registers.A = memory[registers.SP+1];
    registers.SP += 2;
}

void CPU::JMP()
{
    uint8_t lowBits = memory[registers.PC+1];
    uint8_t highBits = memory[registers.PC+2];
    registers.PC = (highBits << 8) + lowBits;
}

void CPU::LXI_B()
{
    registers.C = memory[registers.PC+1];
    registers.B = memory[registers.PC+2];
}

void CPU::LXI_D()
{
    registers.E = memory[registers.PC+1];
    registers.D = memory[registers.PC+2];
}

void CPU::LXI_H()
{
    registers.L = memory[registers.PC+1];
    registers.H = memory[registers.PC+2];
}

void CPU::LXI_SP()
{
    uint8_t lowBits = memory[registers.PC+1];
    uint8_t highBits = memory[registers.PC+2];
    registers.SP = (highBits << 8) + lowBits;
}

void CPU::MVI_B()
{
    registers.B = memory[registers.PC+1];
}

void CPU::MVI_C()
{
    registers.C = memory[registers.PC+1];
}

void CPU::MVI_D()
{
    registers.D = memory[registers.PC+1];
}

void CPU::MVI_E()
{
    registers.E = memory[registers.PC+1];
}

void CPU::MVI_H()
{
    registers.H = memory[registers.PC+1];
}

void CPU::MVI_L()
{
    registers.L = memory[registers.PC+1];
}

void CPU::MVI_M()
{
    int destination = (registers.H << 8) + registers.L;
    memory[destination] = memory[registers.PC+1];
}

void CPU::MVI_A()
{
    registers.A = memory[registers.PC+1];
}

void CPU::CALL()
{
    uint16_t returnPC = registers.PC + 3;
    memory[registers.SP-1] = getHighBits(returnPC);
    memory[registers.SP-2] = getLowBits(returnPC);
    registers.SP -= 2;

    registers.PC = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
}

void CPU::conditionalCall(bool condition)
{
    if(condition)
        CALL();
    else
        registers.PC += 3;
}

void CPU::CC() { conditionalCall(conditionBits.testBits(CARRY_BIT)); }
void CPU::CNC() { conditionalCall(!conditionBits.testBits(CARRY_BIT)); }
void CPU::CZ() { conditionalCall(conditionBits.testBits(ZERO_BIT)); }
void CPU::CNZ() { conditionalCall(!conditionBits.testBits(ZERO_BIT)); }
void CPU::CM() { conditionalCall(conditionBits.testBits(SIGN_BIT)); }
void CPU::CP() { conditionalCall(!conditionBits.testBits(SIGN_BIT)); }
void CPU::CPE() { conditionalCall(conditionBits.testBits(PARITY_BIT)); }
void CPU::CPO() { conditionalCall(!conditionBits.testBits(PARITY_BIT)); }

void CPU::RET()
{
    registers.PC = create16BitReg(memory[registers.SP], memory[registers.SP+1]);
    registers.SP += 2;
}

void CPU::conditionalReturn(bool condition)
{
    if (condition)
        RET();
    else
        registers.PC++;
}

void CPU::RC() { conditionalReturn(conditionBits.testBits(CARRY_BIT)); }
void CPU::RNC() { conditionalReturn(!conditionBits.testBits(CARRY_BIT)); }
void CPU::RZ() { conditionalReturn(conditionBits.testBits(ZERO_BIT)); }
void CPU::RNZ() { conditionalReturn(!conditionBits.testBits(ZERO_BIT)); }
void CPU::RM() { conditionalReturn(conditionBits.testBits(SIGN_BIT)); }
void CPU::RP() { conditionalReturn(!conditionBits.testBits(SIGN_BIT)); }
void CPU::RPE() { conditionalReturn(conditionBits.testBits(PARITY_BIT)); }
void CPU::RPO() { conditionalReturn(!conditionBits.testBits(PARITY_BIT)); }

void CPU::LDA()
{
    uint16_t loadAddr = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
    registers.A = memory[loadAddr];
}

void CPU::STA()
{
    uint16_t storeAddr = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
    memory[storeAddr] = registers.A;
}

void CPU::SHLD()
{
    uint16_t storeAddr = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
    memory[storeAddr] = registers.L;
    memory[storeAddr+1] = registers.H;
}

void CPU::LHLD()
{
    uint16_t loadAddr = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
    registers.L = memory[loadAddr];
    registers.H = memory[loadAddr+1];
}

void CPU::LDAX_B()
{
    uint16_t loadAddr = create16BitReg(registers.C, registers.B);
    registers.A = memory[loadAddr];
}

void CPU::LDAX_D()
{
    uint16_t loadAddr = create16BitReg(registers.E, registers.D);
    registers.A = memory[loadAddr];
}

void CPU::INX_B()
{
  uint16_t regBC = create16BitReg(registers.C, registers.B);
  ++regBC;

  registers.B = getHighBits(regBC);
  registers.C = getLowBits(regBC);
}

void CPU::INX_D()
{
  uint16_t regDE = create16BitReg(registers.E, registers.D);
  ++regDE;

  registers.D = getHighBits(regDE);
  registers.E = getLowBits(regDE);
}

void CPU::INX_H()
{
  uint16_t regHL = create16BitReg(registers.L, registers.H);
  ++regHL;

  registers.H = getHighBits(regHL);
  registers.L = getLowBits(regHL);
}

void CPU::INX_SP()
{
  registers.SP++;
}

void CPU::conditionalJump(bool condition)
{
    if (condition)
        JMP();
    else
        registers.PC += 3;
}

void CPU::JC() { conditionalJump(conditionBits.testBits(CARRY_BIT)); }
void CPU::JNC() { conditionalJump(!conditionBits.testBits(CARRY_BIT)); }
void CPU::JZ() { conditionalJump(conditionBits.testBits(ZERO_BIT)); }
void CPU::JNZ() { conditionalJump(!conditionBits.testBits(ZERO_BIT)); }
void CPU::JM() { conditionalJump(conditionBits.testBits(SIGN_BIT)); }
void CPU::JP() { conditionalJump(!conditionBits.testBits(SIGN_BIT)); }
void CPU::JPE() { conditionalJump(conditionBits.testBits(PARITY_BIT)); }
void CPU::JPO() { conditionalJump(!conditionBits.testBits(PARITY_BIT)); }

void CPU::EI()
{
    interruptsEnabled = true;
}

void CPU::DI()
{
    interruptsEnabled = false;
}

void CPU::RST(uint8_t resetNr)
{
    Q_ASSERT(resetNr >= 0 && resetNr <= 7);

    uint16_t returnPC = registers.PC + 1;
    uint8_t lowPCBits = getLowBits(returnPC);
    uint8_t highPCBits = getHighBits(returnPC);

    memory[registers.SP-1] = highPCBits;
    memory[registers.SP-2] = lowPCBits;
    registers.SP -= 2;

    uint16_t resetAddr = resetNr << 3;
    registers.PC = resetAddr;
}

void CPU::RST_0() { RST(0); }
void CPU::RST_1() { RST(1); }
void CPU::RST_2() { RST(2); }
void CPU::RST_3() { RST(3); }
void CPU::RST_4() { RST(4); }
void CPU::RST_5() { RST(5); }
void CPU::RST_6() { RST(6); }
void CPU::RST_7() { RST(7); }
