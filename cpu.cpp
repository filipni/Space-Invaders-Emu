#include <cpu.h>
#include <cstring>
#include <QtGlobal>
#include <QSound>

CPU::CPU() : conditionBits(), memory()
{
    memset(&registers, 0, sizeof(registers));

    input0 = PORT0_INIT;
    input1 = PORT1_INIT;
    input2 = PORT2_INIT;
    input3 = PORT3_INIT;

    shiftRegister = 0;
    output2 = output3 = output4 = output5 = output6 = 0;

    interruptsEnabled = false;
}

bool CPU::generateInterrupt(uint8_t opCode)
{
    bool success = false;
    if (interruptsEnabled)
    {
        interruptsEnabled = false;
        decode(opCode);
        success = true;
    }
    return success;
}

int CPU::runNextInstruction()
{
    return decode(memory[registers.PC]);
}

uint8_t CPU::getHighBits(uint16_t reg)
{
   return (reg & 0xFF00) >> 8;
}

uint8_t CPU::getHighBits(uint8_t reg)
{
    return (reg & 0xF0) >> 4;
}

uint8_t CPU::getLowBits(uint16_t reg)
{
   return reg & 0x00FF;
}

uint8_t CPU::getLowBits(uint8_t reg)
{
    return reg & 0x0F;
}

uint16_t CPU::create16BitReg(uint8_t lowBits, uint8_t highBits)
{
    return (highBits << 8) + lowBits;
}

int CPU::addBytes(uint8_t byte1, uint8_t byte2, bool carryIn, FlagRegister flagsToCalc)
{
    uint8_t carry = carryIn ? conditionBits.testBits(CARRY_BIT) : 0;

    if (flagsToCalc.testBits(AUX_BIT)) // AUX_BIT = carry from lower to higher nibble
    {
        uint8_t sum = getLowBits(byte1) + getLowBits(byte2) + carry;
        conditionBits.setBits(AUX_BIT, sum >= 0x10);
    }

    uint16_t sum = byte1 + byte2 + carry;
    if (flagsToCalc.testBits(CARRY_BIT))
        conditionBits.setBits(CARRY_BIT, sum >= 0x100);

    // Set the rest of the flags
    if (flagsToCalc.testBits(ZERO_BIT)) conditionBits.calculateZeroBit(sum);
    if (flagsToCalc.testBits(SIGN_BIT)) conditionBits.calculateSignBit(sum);
    if (flagsToCalc.testBits(PARITY_BIT)) conditionBits.calculateEvenParityBit(sum);

    return sum;
}

int CPU::NOP()
{
    registers.PC++;
    return 4;
}

int CPU::CMC()
{
    conditionBits.toggleBits(CARRY_BIT);

    registers.PC++;
    return 4;
}

int CPU::STC()
{
    conditionBits.setBits(CARRY_BIT);

    registers.PC++;
    return 4;
}

int CPU::INR(uint8_t &reg)
{
    FlagRegister flagsToCalc(SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    reg = addBytes(reg, 1, false, flagsToCalc);

    registers.PC++;
    return 5;
}

int CPU::INR_A() { return INR(registers.A); }
int CPU::INR_B() { return INR(registers.B); }
int CPU::INR_C() { return INR(registers.C); }
int CPU::INR_D() { return INR(registers.D); }
int CPU::INR_E() { return INR(registers.E); }
int CPU::INR_H() { return INR(registers.H); }
int CPU::INR_L() { return INR(registers.L); }
int CPU::INR_M() { return INR(memory[create16BitReg(registers.L, registers.H)]); }

int CPU::DCR(uint8_t &reg)
{
    reg = addBytes(reg, -1, false, SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);

    registers.PC++;
    return 5;
}

int CPU::DCR_A() { return DCR(registers.A); }
int CPU::DCR_B() { return DCR(registers.B); }
int CPU::DCR_C() { return DCR(registers.C); }
int CPU::DCR_D() { return DCR(registers.D); }
int CPU::DCR_E() { return DCR(registers.E); }
int CPU::DCR_H() { return DCR(registers.H); }
int CPU::DCR_L() { return DCR(registers.L); }
int CPU::DCR_M() { return DCR(memory[create16BitReg(registers.L, registers.H)]); }

int CPU::CMA()
{
    registers.A = registers.A ^ 0xFF;

    registers.PC++;
    return 4;
}

int CPU::DAA()
{
    int8_t lowerNibble = getLowBits(registers.A);
    if (lowerNibble > 9 || conditionBits.testBits(AUX_BIT))
    {
        FlagRegister flagsToCalc(AUX_BIT);
        registers.A = addBytes(registers.A, 6, false, flagsToCalc);
    }

    if (getHighBits(registers.A) > 9 || conditionBits.testBits(CARRY_BIT))
    {
        bool oldCarry = conditionBits.testBits(CARRY_BIT);

        FlagRegister flagsToCalc(SIGN_BIT | ZERO_BIT | PARITY_BIT | CARRY_BIT);
        registers.A = addBytes(registers.A, 96, false, flagsToCalc);

        if (!conditionBits.testBits(CARRY_BIT)) // Restore old carry if carry flag was unaffected
            conditionBits.setBits(CARRY_BIT, oldCarry);
    }

    registers.PC++;
    return 4;
}

int CPU::MOV_B_B()
{
    registers.B = registers.B;

    registers.PC++;
    return 5;
}

int CPU::MOV_B_C()
{
    registers.B = registers.C;

    registers.PC++;
    return 5;
}


int CPU::MOV_B_D()
{
    registers.B = registers.D;

    registers.PC++;
    return 5;
}

int CPU::MOV_B_E()
{
    registers.B = registers.E;

    registers.PC++;
    return 5;
}

int CPU::MOV_B_H()
{
    registers.B = registers.H;

    registers.PC++;
    return 5;
}

int CPU::MOV_B_L()
{
    registers.B = registers.L;

    registers.PC++;
    return 5;
}

int CPU::MOV_B_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.B = memory[loadAddr];

    registers.PC++;
    return 7;
}

int CPU::MOV_B_A()
{
    registers.B = registers.A;

    registers.PC++;
    return 5;
}

int CPU::MOV_C_B()
{
    registers.C = registers.B;

    registers.PC++;
    return 5;
}

int CPU::MOV_C_C()
{
    registers.C = registers.C;

    registers.PC++;
    return 5;
}

int CPU::MOV_C_D()
{
    registers.C = registers.D;

    registers.PC++;
    return 5;
}

int CPU::MOV_C_E()
{
    registers.C = registers.E;

    registers.PC++;
    return 5;
}

int CPU::MOV_C_H()
{
    registers.C = registers.H;

    registers.PC++;
    return 5;
}

int CPU::MOV_C_L()
{
    registers.C = registers.L;

    registers.PC++;
    return 5;
}

int CPU::MOV_C_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.C = memory[loadAddr];

    registers.PC++;
    return 7;
}

int CPU::MOV_C_A()
{
    registers.C = registers.A;

    registers.PC++;
    return 5;
}

int CPU::MOV_D_B()
{
    registers.D = registers.B;

    registers.PC++;
    return 5;
}

int CPU::MOV_D_C()
{
    registers.D = registers.C;

    registers.PC++;
    return 5;
}

int CPU::MOV_D_D()
{
    registers.D = registers.D;

    registers.PC++;
    return 5;
}

int CPU::MOV_D_E()
{
    registers.D = registers.E;

    registers.PC++;
    return 5;
}

int CPU::MOV_D_H()
{
    registers.D = registers.H;

    registers.PC++;
    return 5;
}

int CPU::MOV_D_L()
{
    registers.D = registers.L;

    registers.PC++;
    return 5;
}

int CPU::MOV_D_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.D = memory[loadAddr];

    registers.PC++;
    return 7;
}

int CPU::MOV_D_A()
{
    registers.D = registers.A;

    registers.PC++;
    return 5;
}

int CPU::MOV_E_B()
{
    registers.E = registers.B;

    registers.PC++;
    return 5;
}

int CPU::MOV_E_C()
{
    registers.E = registers.C;

    registers.PC++;
    return 5;
}

int CPU::MOV_E_D()
{
    registers.E = registers.D;

    registers.PC++;
    return 5;
}

int CPU::MOV_E_E()
{
    registers.E = registers.E;

    registers.PC++;
    return 5;
}

int CPU::MOV_E_H()
{
    registers.E = registers.H;

    registers.PC++;
    return 5;
}

int CPU::MOV_E_L()
{
    registers.E = registers.L;

    registers.PC++;
    return 5;
}

int CPU::MOV_E_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.E = memory[loadAddr];

    registers.PC++;
    return 7;
}

int CPU::MOV_E_A()
{
    registers.E = registers.A;

    registers.PC++;
    return 5;
}

int CPU::MOV_H_B()
{
    registers.H = registers.B;

    registers.PC++;
    return 5;
}

int CPU::MOV_H_C()
{
    registers.H = registers.C;

    registers.PC++;
    return 5;
}

int CPU::MOV_H_D()
{
    registers.H = registers.D;

    registers.PC++;
    return 5;
}

int CPU::MOV_H_E()
{
    registers.H = registers.E;

    registers.PC++;
    return 5;
}

int CPU::MOV_H_H()
{
    registers.H = registers.H;

    registers.PC++;
    return 5;
}

int CPU::MOV_H_L()
{
    registers.H = registers.L;

    registers.PC++;
    return 5;
}

int CPU::MOV_H_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.H = memory[loadAddr];

    registers.PC++;
    return 7;
}

int CPU::MOV_H_A()
{
    registers.H = registers.A;

    registers.PC++;
    return 5;
}

int CPU::MOV_L_B()
{
    registers.L = registers.B;

    registers.PC++;
    return 5;
}

int CPU::MOV_L_C()
{
    registers.L = registers.C;

    registers.PC++;
    return 5;
}

int CPU::MOV_L_D()
{
    registers.L = registers.D;

    registers.PC++;
    return 5;
}

int CPU::MOV_L_E()
{
    registers.L = registers.E;

    registers.PC++;
    return 5;
}

int CPU::MOV_L_H()
{
    registers.L = registers.H;

    registers.PC++;
    return 5;
}

int CPU::MOV_L_L()
{
    registers.L = registers.L;

    registers.PC++;
    return 5;
}

int CPU::MOV_L_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.L = memory[loadAddr];

    registers.PC++;
    return 7;
}

int CPU::MOV_L_A()
{
    registers.L = registers.A;

    registers.PC++;
    return 5;
}

int CPU::MOV_M_B()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.B;

    registers.PC++;
    return 7;
}

int CPU::MOV_M_C()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.C;

    registers.PC++;
    return 7;
}

int CPU::MOV_M_D()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.D;

    registers.PC++;
    return 7;
}

int CPU::MOV_M_E()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.E;

    registers.PC++;
    return 7;
}

int CPU::MOV_M_H()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.H;

    registers.PC++;
    return 7;
}

int CPU::MOV_M_L()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.L;

    registers.PC++;
    return 7;
}

int CPU::MOV_M_A()
{
    int storeAddr = create16BitReg(registers.L, registers.H);
    memory[storeAddr] = registers.A;

    registers.PC++;
    return 7;
}

int CPU::MOV_A_B()
{
    registers.A = registers.B;

    registers.PC++;
    return 5;
}

int CPU::MOV_A_C()
{
    registers.A = registers.C;

    registers.PC++;
    return 5;
}

int CPU::MOV_A_D()
{
    registers.A = registers.D;

    registers.PC++;
    return 5;
}

int CPU::MOV_A_E()
{
    registers.A = registers.E;

    registers.PC++;
    return 5;
}

int CPU::MOV_A_H()
{
    registers.A = registers.H;

    registers.PC++;
    return 5;
}

int CPU::MOV_A_L()
{
    registers.A = registers.L;

    registers.PC++;
    return 5;
}

int CPU::MOV_A_M()
{
    int loadAddr = create16BitReg(registers.L, registers.H);
    registers.A = memory[loadAddr];

    registers.PC++;
    return 7;
}

int CPU::MOV_A_A()
{
    registers.A = registers.A;

    registers.PC++;
    return 5;
}

int CPU::ADD(uint8_t operand)
{
    FlagRegister flagsToCalc(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    registers.A = addBytes(registers.A, operand, false, flagsToCalc);

    registers.PC++;
    return 4;
}

int CPU::ADD_B() { return ADD(registers.B); }
int CPU::ADD_C() { return ADD(registers.C); }
int CPU::ADD_D() { return ADD(registers.D); }
int CPU::ADD_E() { return ADD(registers.E); }
int CPU::ADD_H() { return ADD(registers.H); }
int CPU::ADD_L() { return ADD(registers.L); }
int CPU::ADD_A() { return ADD(registers.L); }
int CPU::ADD_M()
{
    ADD(memory[create16BitReg(registers.L, registers.H)]);
    return 7;
}

int CPU::ADC(uint8_t operand)
{
    FlagRegister flagsToCalc(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    registers.A = addBytes(registers.A, operand, true, flagsToCalc);

    registers.PC++;
    return 4;
}

int CPU::ADC_B() { return ADC(registers.B); }
int CPU::ADC_C() { return ADC(registers.C); }
int CPU::ADC_D() { return ADC(registers.D); }
int CPU::ADC_E() { return ADC(registers.E); }
int CPU::ADC_H() { return ADC(registers.H); }
int CPU::ADC_L() { return ADC(registers.L); }
int CPU::ADC_A() { return ADC(registers.A); }
int CPU::ADC_M()
{
    ADC(memory[create16BitReg(registers.L, registers.H)]);
    return 7;
}

int CPU::SUB(uint8_t operand)
{
    FlagRegister flagsToCalc(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    int8_t operand2Cmp = (operand ^ 0xFF) + 1;
    registers.A = addBytes(registers.A, operand2Cmp, false, flagsToCalc);
    conditionBits.setBits(CARRY_BIT, !conditionBits.testBits(CARRY_BIT)); // Since this is a substraction, we invert the carry

    registers.PC++;
    return 4;
}

int CPU::SUB_B() { return SUB(registers.B); }
int CPU::SUB_C() { return SUB(registers.C); }
int CPU::SUB_D() { return SUB(registers.D); }
int CPU::SUB_E() { return SUB(registers.E); }
int CPU::SUB_H() { return SUB(registers.H); }
int CPU::SUB_L() { return SUB(registers.L); }
int CPU::SUB_A() { return SUB(registers.A); }
int CPU::SUB_M()
{
    SUB(memory[create16BitReg(registers.L, registers.H)]);
    return 7;
}

int CPU::SBB(uint8_t operand)
{
    FlagRegister flagsToCalc(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    operand = operand + conditionBits.testBits(CARRY_BIT);
    int8_t operand2Cmp = (operand ^ 0xFF) + 1;
    registers.A = addBytes(registers.A, operand2Cmp, false, flagsToCalc);
    conditionBits.setBits(CARRY_BIT, !conditionBits.testBits(CARRY_BIT)); // Since this is a substraction, we invert the carry

    registers.PC++;
    return 4;
}

int CPU::SBB_B() { return SBB(registers.B); }
int CPU::SBB_C() { return SBB(registers.C); }
int CPU::SBB_D() { return SBB(registers.D); }
int CPU::SBB_E() { return SBB(registers.E); }
int CPU::SBB_H() { return SBB(registers.H); }
int CPU::SBB_L() { return SBB(registers.L); }
int CPU::SBB_A() { return SBB(registers.A); }
int CPU::SBB_M()
{
    SBB(memory[create16BitReg(registers.L, registers.H)]);
    return 7;
}

int CPU::ANA(uint8_t operand)
{
    registers.A &= operand;
    conditionBits.setBits(CARRY_BIT, false);
    conditionBits.calculateZeroSignParityBits(registers.A);

    registers.PC++;
    return 4;
}

int CPU::ANA_B() { return ANA(registers.B); }
int CPU::ANA_C() { return ANA(registers.C); }
int CPU::ANA_D() { return ANA(registers.D); }
int CPU::ANA_E() { return ANA(registers.E); }
int CPU::ANA_H() { return ANA(registers.H); }
int CPU::ANA_L() { return ANA(registers.L); }
int CPU::ANA_A() { return ANA(registers.A); }
int CPU::ANA_M()
{
    ANA(memory[create16BitReg(registers.L, registers.H)]);
    return 7;
}

int CPU::XRA(int8_t operand)
{
    registers.A ^= operand;
    conditionBits.setBits(CARRY_BIT, false);
    conditionBits.calculateZeroSignParityBits(registers.A);

    registers.PC++;
    return 4;
}

int CPU::XRA_B() { return XRA(registers.B); }
int CPU::XRA_C() { return XRA(registers.C); }
int CPU::XRA_D() { return XRA(registers.D); }
int CPU::XRA_E() { return XRA(registers.E); }
int CPU::XRA_H() { return XRA(registers.H); }
int CPU::XRA_L() { return XRA(registers.L); }
int CPU::XRA_A() { return XRA(registers.A); }
int CPU::XRA_M()
{
    XRA(memory[create16BitReg(registers.L, registers.H)]);
    return 7;
}

int CPU::ORA(uint8_t operand)
{
    registers.A |= operand;
    conditionBits.setBits(CARRY_BIT, false);
    conditionBits.calculateZeroSignParityBits(registers.A);

    registers.PC++;
    return 4;
}

int CPU::ORA_B() { return ORA(registers.B); }
int CPU::ORA_C() { return ORA(registers.C); }
int CPU::ORA_D() { return ORA(registers.D); }
int CPU::ORA_E() { return ORA(registers.E); }
int CPU::ORA_H() { return ORA(registers.H); }
int CPU::ORA_L() { return ORA(registers.L); }
int CPU::ORA_A() { return ORA(registers.A); }
int CPU::ORA_M()
{
    ORA(memory[create16BitReg(registers.L, registers.H)]);
    return 7;
}

int CPU::CMP(uint8_t reg)
{
    FlagRegister flagsToCalc(SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT | CARRY_BIT);
    addBytes(registers.A, -reg, false, flagsToCalc);

    conditionBits.setBits(CARRY_BIT, !conditionBits.testBits(CARRY_BIT)); // Since this is a substraction, we invert the carry

    registers.PC++;
    return 4;
}

int CPU::CMP_B() { return CMP(registers.B); }
int CPU::CMP_C() { return CMP(registers.C); }
int CPU::CMP_D() { return CMP(registers.D); }
int CPU::CMP_E() { return CMP(registers.E); }
int CPU::CMP_H() { return CMP(registers.H); }
int CPU::CMP_L() { return CMP(registers.L); }
int CPU::CMP_A() { return CMP(registers.A); }
int CPU::CMP_M()
{
    CMP(memory[create16BitReg(registers.L, registers.H)]);
    return 7;
}

int CPU::RLC()
{
    uint8_t carry = (registers.A & HIGH_ORDER_BIT) >> 7;
    conditionBits.setBits(CARRY_BIT, carry);
    registers.A <<= 1;
    registers.A |= carry;

    registers.PC++;
    return 4;
}

int CPU::RRC()
{
    uint8_t carry = registers.A & LOW_ORDER_BIT;
    conditionBits.setBits(CARRY_BIT, carry);
    registers.A >>= 1;
    registers.A = registers.A | (carry << 7);

    registers.PC++;
    return 4;
}

int CPU::RAL()
{
    uint8_t newCarry = (registers.A & HIGH_ORDER_BIT) >> 7;
    uint8_t oldCarry = conditionBits.testBits(CARRY_BIT);
    conditionBits.setBits(CARRY_BIT, newCarry);
    registers.A <<= 1;
    registers.A |= oldCarry;

    registers.PC++;
    return 4;
}

int CPU::RAR()
{
    uint8_t newCarry = registers.A & LOW_ORDER_BIT;
    uint8_t oldCarry = conditionBits.testBits(CARRY_BIT);
    conditionBits.setBits(CARRY_BIT, newCarry);
    registers.A >>= 1;
    registers.A = registers.A | (oldCarry << 7);

    registers.PC++;
    return 4;
}

int CPU::PUSH_B()
{
    memory[registers.SP-1] = registers.B;
    memory[registers.SP-2] = registers.C;
    registers.SP -= 2;

    registers.PC++;
    return 11;
}

int CPU::PUSH_D()
{
    memory[registers.SP-1] = registers.D;
    memory[registers.SP-2] = registers.E;
    registers.SP -= 2;

    registers.PC++;
    return 11;
}

int CPU::PUSH_H()
{
    memory[registers.SP-1] = registers.H;
    memory[registers.SP-2] = registers.L;
    registers.SP -= 2;

    registers.PC++;
    return 11;
}

int CPU::PUSH_PSW()
{
    memory[registers.SP-1] = registers.A;
    memory[registers.SP-2] = conditionBits.getRegister();
    registers.SP -= 2;

    registers.PC++;
    return 11;
}

int CPU::POP_B()
{
    registers.C = memory[registers.SP];
    registers.B = memory[registers.SP+1];
    registers.SP += 2;

    registers.PC++;
    return 10;
}

int CPU::POP_D()
{
    registers.E = memory[registers.SP];
    registers.D = memory[registers.SP+1];
    registers.SP += 2;

    registers.PC++;
    return 10;
}

int CPU::POP_H()
{
    registers.L = memory[registers.SP];
    registers.H = memory[registers.SP+1];
    registers.SP += 2;

    registers.PC++;
    return 10;
}

int CPU::POP_PSW()
{
    conditionBits = FlagRegister(memory[registers.SP]);
    registers.A = memory[registers.SP+1];
    registers.SP += 2;

    registers.PC++;
    return 10;
}

int CPU::JMP()
{
    uint8_t lowBits = memory[registers.PC+1];
    uint8_t highBits = memory[registers.PC+2];
    registers.PC = create16BitReg(lowBits, highBits);

    return 10;
}

int CPU::LXI_B()
{
    registers.C = memory[registers.PC+1];
    registers.B = memory[registers.PC+2];

    registers.PC += 3;
    return 10;
}

int CPU::LXI_D()
{
    registers.E = memory[registers.PC+1];
    registers.D = memory[registers.PC+2];

    registers.PC += 3;
    return 10;
}

int CPU::LXI_H()
{
    registers.L = memory[registers.PC+1];
    registers.H = memory[registers.PC+2];

    registers.PC += 3;
    return 10;
}

int CPU::LXI_SP()
{
    uint8_t lowBits = memory[registers.PC+1];
    uint8_t highBits = memory[registers.PC+2];
    registers.SP = create16BitReg(lowBits, highBits);

    registers.PC += 3;
    return 10;
}

int CPU::MVI_B()
{
    registers.B = memory[registers.PC+1];

    registers.PC += 2;
    return 7;
}

int CPU::MVI_C()
{
    registers.C = memory[registers.PC+1];

    registers.PC += 2;
    return 7;
}

int CPU::MVI_D()
{
    registers.D = memory[registers.PC+1];

    registers.PC += 2;
    return 7;
}

int CPU::MVI_E()
{
    registers.E = memory[registers.PC+1];

    registers.PC += 2;
    return 7;
}

int CPU::MVI_H()
{
    registers.H = memory[registers.PC+1];

    registers.PC += 2;
    return 7;
}

int CPU::MVI_L()
{
    registers.L = memory[registers.PC+1];

    registers.PC += 2;
    return 7;
}

int CPU::MVI_M()
{
    int destination = create16BitReg(registers.L, registers.H);
    memory[destination] = memory[registers.PC+1];

    registers.PC += 2;
    return 10;
}

int CPU::MVI_A()
{
    registers.A = memory[registers.PC+1];

    registers.PC += 2;
    return 7;
}

int CPU::CALL()
{
    uint16_t returnPC = registers.PC + 3;
    memory[registers.SP-1] = getHighBits(returnPC);
    memory[registers.SP-2] = getLowBits(returnPC);
    registers.SP -= 2;

    registers.PC = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
    return 17;
}

double CPU::conditionalCall(bool condition)
{
    if(condition)
    {
        CALL();
        return 17;
    }
    else
    {
        registers.PC += 3;
        return 11;
    }
}

double CPU::CC() { return conditionalCall(conditionBits.testBits(CARRY_BIT)); }
double CPU::CNC() { return conditionalCall(!conditionBits.testBits(CARRY_BIT)); }
double CPU::CZ() { return conditionalCall(conditionBits.testBits(ZERO_BIT)); }
double CPU::CNZ() { return conditionalCall(!conditionBits.testBits(ZERO_BIT)); }
double CPU::CM() { return conditionalCall(conditionBits.testBits(SIGN_BIT)); }
double CPU::CP() { return conditionalCall(!conditionBits.testBits(SIGN_BIT)); }
double CPU::CPE() { return conditionalCall(conditionBits.testBits(PARITY_BIT)); }
double CPU::CPO() { return conditionalCall(!conditionBits.testBits(PARITY_BIT)); }

int CPU::RET()
{
    registers.PC = create16BitReg(memory[registers.SP], memory[registers.SP+1]);
    registers.SP += 2;

    return 10;
}

double CPU::conditionalReturn(bool condition)
{
    if (condition)
    {
        RET();
        return 11;
    }
    else
    {
        registers.PC++;
        return 5;
    }
}

double CPU::RC() { return conditionalReturn(conditionBits.testBits(CARRY_BIT)); }
double CPU::RNC() { return conditionalReturn(!conditionBits.testBits(CARRY_BIT)); }
double CPU::RZ() { return conditionalReturn(conditionBits.testBits(ZERO_BIT)); }
double CPU::RNZ() { return conditionalReturn(!conditionBits.testBits(ZERO_BIT)); }
double CPU::RM() { return conditionalReturn(conditionBits.testBits(SIGN_BIT)); }
double CPU::RP() { return conditionalReturn(!conditionBits.testBits(SIGN_BIT)); }
double CPU::RPE() { return conditionalReturn(conditionBits.testBits(PARITY_BIT)); }
double CPU::RPO() { return conditionalReturn(!conditionBits.testBits(PARITY_BIT)); }

int CPU::LDA()
{
    uint16_t loadAddr = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
    registers.A = memory[loadAddr];

    registers.PC += 3;
    return 13;
}

int CPU::STA()
{
    uint16_t storeAddr = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
    memory[storeAddr] = registers.A;

    registers.PC += 3;
    return 13;
}

int CPU::SHLD()
{
    uint16_t storeAddr = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
    memory[storeAddr] = registers.L;
    memory[storeAddr+1] = registers.H;

    registers.PC += 3;
    return 16;
}

int CPU::LHLD()
{
    uint16_t loadAddr = create16BitReg(memory[registers.PC+1], memory[registers.PC+2]);
    registers.L = memory[loadAddr];
    registers.H = memory[loadAddr+1];

    registers.PC += 3;
    return 16;
}

int CPU::LDAX_B()
{
    uint16_t loadAddr = create16BitReg(registers.C, registers.B);
    registers.A = memory[loadAddr];

    registers.PC++;
    return 7;
}

int CPU::LDAX_D()
{
    uint16_t loadAddr = create16BitReg(registers.E, registers.D);
    registers.A = memory[loadAddr];

    registers.PC++;
    return 7;
}

int CPU::INX_B()
{
    uint16_t regBC = create16BitReg(registers.C, registers.B);
    ++regBC;

    registers.B = getHighBits(regBC);
    registers.C = getLowBits(regBC);

    registers.PC++;
    return 5;
}

int CPU::INX_D()
{
    uint16_t regDE = create16BitReg(registers.E, registers.D);
    ++regDE;

    registers.D = getHighBits(regDE);
    registers.E = getLowBits(regDE);

    registers.PC++;
    return 5;
}

int CPU::INX_H()
{
    uint16_t regHL = create16BitReg(registers.L, registers.H);
    ++regHL;

    registers.H = getHighBits(regHL);
    registers.L = getLowBits(regHL);

    registers.PC++;
    return 5;
}

int CPU::INX_SP()
{
    registers.SP++;

    registers.PC++;
    return 5;
}

int CPU::DCX_B()
{
    uint16_t regBC = create16BitReg(registers.C, registers.B);
    --regBC;

    registers.B = getHighBits(regBC);
    registers.C = getLowBits(regBC);

    registers.PC++;
    return 5;
}

int CPU::DCX_D()
{
    uint16_t regDE = create16BitReg(registers.E, registers.D);
    --regDE;

    registers.D = getHighBits(regDE);
    registers.E = getLowBits(regDE);

    registers.PC++;
    return 5;
}

int CPU::DCX_H()
{
    uint16_t regHL = create16BitReg(registers.L, registers.H);
    --regHL;

    registers.H = getHighBits(regHL);
    registers.L = getLowBits(regHL);

    registers.PC++;
    return 5;
}

int CPU::DCX_SP()
{
    registers.SP--;

    registers.PC++;
    return 5;
}

int CPU::conditionalJump(bool condition)
{
    if (condition)
        JMP();
    else
        registers.PC += 3;

    return 10;
}

int CPU::JC() { return conditionalJump(conditionBits.testBits(CARRY_BIT)); }
int CPU::JNC() { return conditionalJump(!conditionBits.testBits(CARRY_BIT)); }
int CPU::JZ() { return conditionalJump(conditionBits.testBits(ZERO_BIT)); }
int CPU::JNZ() { return conditionalJump(!conditionBits.testBits(ZERO_BIT)); }
int CPU::JM() { return conditionalJump(conditionBits.testBits(SIGN_BIT)); }
int CPU::JP() { return conditionalJump(!conditionBits.testBits(SIGN_BIT)); }
int CPU::JPE() { return conditionalJump(conditionBits.testBits(PARITY_BIT)); }
int CPU::JPO() { return conditionalJump(!conditionBits.testBits(PARITY_BIT)); }

int CPU::EI()
{
    interruptsEnabled = true;

    registers.PC++;
    return 4;
}

int CPU::DI()
{
    interruptsEnabled = false;

    registers.PC++;
    return 4;
}

int CPU::RST(uint8_t resetNr)
{
    Q_ASSERT(resetNr <= 7);

    uint8_t lowPCBits = getLowBits(registers.PC);
    uint8_t highPCBits = getHighBits(registers.PC);

    memory[registers.SP-1] = highPCBits;
    memory[registers.SP-2] = lowPCBits;
    registers.SP -= 2;

    uint16_t resetAddr = resetNr << 3;
    registers.PC = resetAddr;

    return 11;
}

int CPU::RST_0() { return RST(0); }
int CPU::RST_1() { return RST(1); }
int CPU::RST_2() { return RST(2); }
int CPU::RST_3() { return RST(3); }
int CPU::RST_4() { return RST(4); }
int CPU::RST_5() { return RST(5); }
int CPU::RST_6() { return RST(6); }
int CPU::RST_7() { return RST(7); }

int CPU::IN()
{
    uint8_t inputNr = memory[registers.PC+1];

    switch (inputNr)
    {
      case 1:
        registers.A = input1;
        break;
      case 2:
        registers.A = input2;
        break;
      case 3:
        registers.A = input3;
        break;
      default:
        qDebug() << "Inupt nr " << inputNr << " not implemented";
    }

    registers.PC += 2;
    return 10;
}

int CPU::OUT()
{
    uint8_t outputNr = memory[registers.PC+1];

    switch (outputNr)
    {
      case 2:
        output2 = registers.A;
        break;
      case 3:
        output3 = registers.A;
        break;
      case 4:
        output4 = registers.A;
        shiftRegisterOp();
        break;
      case 5:
        output5 = registers.A;
        break;
      case 6:
        output6 = registers.A;
        break;
      default:
        qDebug() << "Output nr " << outputNr << " not implemented";
    }

    registers.PC += 2;
    return 10;
}

void CPU::shiftRegisterOp()
{
    uint8_t offset = output2 & 7;  // Offset is kept in the three first bits

    shiftRegister >>= 8; // Make room for the new value

    // Add the new value to the upper byte of the shift register
    uint16_t shiftIn = output4 << 8;
    shiftRegister |= shiftIn;

    // Pick out the result and convert it to an 8-bit value
    uint16_t resultBitMask = 0xFF00 >> offset;
    input3 = (shiftRegister & resultBitMask) >> (8 - offset);
}

int CPU::ADI() { ADD(memory[registers.PC+1]); registers.PC++; return 7;}
int CPU::SUI() { SUB(memory[registers.PC+1]); registers.PC++; return 7;}
int CPU::ANI() { ANA(memory[registers.PC+1]); registers.PC++; return 7;}
int CPU::ORI() { ORA(memory[registers.PC+1]); registers.PC++; return 7;}
int CPU::ACI() { ADC(memory[registers.PC+1]); registers.PC++; return 7;}
int CPU::SBI() { SBB(memory[registers.PC+1]); registers.PC++; return 7;}
int CPU::XRI() { XRA(memory[registers.PC+1]); registers.PC++; return 7;}
int CPU::CPI() { CMP(memory[registers.PC+1]); registers.PC++; return 7;}

int CPU::DAD(uint8_t highReg, uint8_t lowReg)
{
    uint16_t operandReg = create16BitReg(lowReg, highReg);
    uint16_t regHL = create16BitReg(registers.L, registers.H);
    int32_t result = operandReg + regHL;

    if (result & 0x10000)
        conditionBits.setBits(CARRY_BIT);

    registers.H = getHighBits( (uint16_t) result);
    registers.L = getLowBits( (uint16_t) result);

    registers.PC++;
    return 10;
}

int CPU::DAD_SP()
{
    uint16_t regHL = create16BitReg(registers.L, registers.H);
    int32_t result = registers.SP + regHL;

    if (result & 0x10000)
        conditionBits.setBits(CARRY_BIT);

    registers.H = getHighBits( (uint16_t) result);
    registers.L = getLowBits( (uint16_t) result);

    registers.PC++;
    return 10;
}

int CPU::DAD_B() { return DAD(registers.B, registers.C); }
int CPU::DAD_D() { return DAD(registers.D, registers.E); }
int CPU::DAD_H() { return DAD(registers.H, registers.L); }

int CPU::STAX_B()
{
    uint16_t storeAddr = create16BitReg(registers.C, registers.B);
    memory[storeAddr] = registers.A;

    registers.PC++;
    return 7;
}

int CPU::STAX_D()
{
    uint16_t storeAddr = create16BitReg(registers.E, registers.D);
    memory[storeAddr] = registers.A;

    registers.PC++;
    return 7;
}

int CPU::PCHL()
{
    registers.PC = create16BitReg(registers.L, registers.H);
    return 5;
}

int CPU::XCHG()
{
    uint16_t regDE = create16BitReg(registers.E, registers.D);
    uint16_t regHL = create16BitReg(registers.L, registers.H);

    registers.D = getHighBits(regHL);
    registers.E = getLowBits(regHL);

    registers.H = getHighBits(regDE);
    registers.L = getLowBits(regDE);

    registers.PC++;
    return 5;
}

int CPU::XTHL()
{
    uint8_t regH = registers.H;
    uint8_t regL = registers.L;

    registers.L = memory[registers.SP];
    registers.H = memory[registers.SP+1];

    memory[registers.SP] = regL;
    memory[registers.SP+1] = regH;

    registers.PC++;
    return 18;
}

int CPU::SPHL()
{
    registers.SP = create16BitReg(registers.L, registers.H);

    registers.PC++;
    return 5;
}

int CPU::decode(uint8_t op)
{
    switch(op)
    {
      case 0x00:
          return NOP();
      case 0x01:
          return LXI_B();
      case 0x02:
          return STAX_B();
      case 0x03:
          return INX_B();
      case 0x04:
          return INR_B();
      case 0x05:
          return DCR_B();
      case 0x06:
          return MVI_B();
      case 0x07:
          return RLC();
      case 0x08:
          return NOP();
      case 0x09:
          return DAD_B();
      case 0x0A:
          return LDAX_B();
      case 0x0B:
          return DCX_B();
      case 0x0C:
          return INR_C();
      case 0x0D:
          return DCR_C();
      case 0x0E:
          return MVI_C();
      case 0x0F:
          return RRC();
      case 0x10:
          return NOP();
      case 0x11:
          return LXI_D();
      case 0x12:
          return STAX_D();
      case 0x13:
          return INX_D();
      case 0x14:
          return INR_D();
      case 0x15:
          return DCR_D();
      case 0x16:
          return MVI_D();
      case 0x17:
          return RAL();
      case 0x18:
          return NOP();
      case 0x19:
          return DAD_D();
      case 0x1A:
          return LDAX_D();
      case 0x1B:
          return DCX_D();
      case 0x1C:
          return INR_E();
      case 0x1D:
          return DCR_E();
      case 0x1E:
          return MVI_C();
      case 0x1F:
          return RAR();
      case 0x20:
          return NOP();
      case 0x21:
          return LXI_H();
      case 0x22:
          return SHLD();
      case 0x23:
          return INX_H();
      case 0x24:
          return INR_H();
      case 0x25:
          return DCR_H();
      case 0x26:
          return MVI_H();
      case 0x27:
          return DAA();
      case 0x28:
          return NOP();
      case 0x29:
          return DAD_H();
      case 0x2A:
          return LHLD();
      case 0x2B:
          return DCX_H();
      case 0x2C:
          return INR_L();
      case 0x2D:
          return DCR_L();
      case 0x2E:
          return MVI_L();
      case 0x2F:
          return CMA();
      case 0x30:
          return NOP();
      case 0x31:
          return LXI_SP();
      case 0x32:
          return STA();
      case 0x33:
          return INX_SP();
      case 0x34:
          return INR_M();
      case 0x35:
          return DCR_M();
      case 0x36:
          return MVI_M();
      case 0x37:
          return STC();
      case 0x38:
          return NOP();
      case 0x39:
          return DAD_SP();
      case 0x3A:
          return LDA();
      case 0x3B:
          return DCX_SP();
      case 0x3C:
          return INR_A();
      case 0x3D:
          return DCR_A();
      case 0x3E:
          return MVI_A();
      case 0x3F:
          return CMC();
      case 0x40:
          return MOV_B_B();
      case 0x41:
          return MOV_B_C();
      case 0x42:
          return MOV_B_D();
      case 0x43:
          return MOV_B_E();
      case 0x44:
          return MOV_B_H();
      case 0x45:
          return MOV_B_L();
      case 0x46:
          return MOV_B_M();
      case 0x47:
          return MOV_B_A();
      case 0x48:
          return MOV_C_B();
      case 0x49:
          return MOV_C_C();
      case 0x4A:
          return MOV_C_D();
      case 0x4B:
          return MOV_C_E();
      case 0x4C:
          return MOV_C_H();
      case 0x4D:
          return MOV_C_L();
      case 0x4E:
          return MOV_C_M();
      case 0x4F:
          return MOV_C_A();
      case 0x50:
          return MOV_D_B();
      case 0x51:
          return MOV_D_C();
      case 0x52:
          return MOV_D_D();
      case 0x53:
          return MOV_D_E();
      case 0x54:
          return MOV_D_H();
      case 0x55:
          return MOV_D_L();
      case 0x56:
          return MOV_D_M();
      case 0x57:
          return MOV_D_A();
      case 0x58:
          return MOV_E_B();
      case 0x59:
          return MOV_E_C();
      case 0x5A:
          return MOV_E_D();
      case 0x5B:
          return MOV_E_E();
      case 0x5C:
          return MOV_E_H();
      case 0x5D:
          return MOV_E_L();
      case 0x5E:
          return MOV_E_M();
      case 0x5F:
          return MOV_E_A();
      case 0x60:
          return MOV_H_B();
      case 0x61:
          return MOV_H_C();
      case 0x62:
          return MOV_H_D();
      case 0x63:
          return MOV_H_E();
      case 0x64:
          return MOV_H_H();
      case 0x65:
          return MOV_H_L();
      case 0x66:
          return MOV_H_M();
      case 0x67:
          return MOV_H_A();
      case 0x68:
          return MOV_L_B();
      case 0x69:
          return MOV_L_C();
      case 0x6A:
          return MOV_L_D();
      case 0x6B:
          return MOV_L_E();
      case 0x6C:
          return MOV_L_H();
      case 0x6D:
          return MOV_L_L();
      case 0x6E:
          return MOV_L_M();
      case 0x6F:
          return MOV_L_A();
      case 0x70:
          return MOV_M_B();
      case 0x71:
          return MOV_M_C();
      case 0x72:
          return MOV_M_D();
      case 0x73:
          return MOV_M_E();
      case 0x74:
          return MOV_M_H();
      case 0x75:
          return MOV_M_L();
      case 0x76:
          //HLT();
          qDebug() << "HLT instruction not implemented yet";
          registers.PC +=1;
          return 7;
      case 0x77:
          return MOV_M_A();
      case 0x78:
          return MOV_A_B();
      case 0x79:
          return MOV_A_C();
      case 0x7A:
          return MOV_A_D();
      case 0x7B:
          return MOV_A_E();
      case 0x7C:
          return MOV_A_H();
      case 0x7D:
          return MOV_A_L();
      case 0x7E:
          return MOV_A_M();
      case 0x7F:
          return MOV_A_A();
      case 0x80:
          return ADD_B();
      case 0x81:
          return ADD_C();
      case 0x82:
          return ADD_D();
      case 0x83:
          return ADD_E();
      case 0x84:
          return ADD_H();
      case 0x85:
          return ADD_L();
      case 0x86:
          return ADD_M();
      case 0x87:
          return ADD_A();
      case 0x88:
          return ADC_B();
      case 0x89:
          return ADC_C();
      case 0x8A:
          return ADC_D();
      case 0x8B:
          return ADC_E();
      case 0x8C:
          return ADC_H();
      case 0x8D:
          return ADC_L();
      case 0x8E:
          return ADC_M();
      case 0x8F:
          return ADC_A();
      case 0x90:
          return SUB_B();
      case 0x91:
          return SUB_C();
      case 0x92:
          return SUB_D();
      case 0x93:
          return SUB_E();
      case 0x94:
          return SUB_H();
      case 0x95:
          return SUB_L();
      case 0x96:
          return SUB_M();
      case 0x97:
          return SUB_A();
      case 0x98:
          return SBB_B();
      case 0x99:
          return SBB_C();
      case 0x9A:
          return SBB_D();
      case 0x9B:
          return SBB_E();
      case 0x9C:
          return SBB_H();
      case 0x9D:
          return SBB_L();
      case 0x9E:
          return SBB_M();
      case 0x9F:
          return SBB_A();
      case 0xA0:
          return ANA_B();
      case 0xA1:
          return ANA_C();
      case 0xA2:
          return ANA_D();
      case 0xA3:
          return ANA_E();
      case 0xA4:
          return ANA_H();
      case 0xA5:
          return ANA_L();
      case 0xA6:
          return ANA_M();
      case 0xA7:
          return ANA_A();
      case 0xA8:
          return XRA_B();
      case 0xA9:
          return XRA_C();
      case 0xAA:
          return XRA_D();
      case 0xAB:
          return XRA_E();
      case 0xAC:
          return XRA_H();
      case 0xAD:
          return XRA_L();
      case 0xAE:
          return XRA_M();
      case 0xAF:
          return XRA_A();
      case 0xB0:
          return ORA_B();
      case 0xB1:
          return ORA_C();
      case 0xB2:
          return ORA_D();
      case 0xB3:
          return ORA_E();
      case 0xB4:
          return ORA_H();
      case 0xB5:
          return ORA_L();
      case 0xB6:
          return ORA_M();
      case 0xB7:
          return ORA_A();
      case 0xB8:
          return CMP_B();
      case 0xB9:
          return CMP_C();
      case 0xBA:
          return CMP_D();
      case 0xBB:
          return CMP_E();
      case 0xBC:
          return CMP_H();
      case 0xBD:
          return CMP_L();
      case 0xBE:
          return CMP_M();
      case 0xBF:
          return CMP_A();
      case 0xC0:
          return RNZ();
      case 0xC1:
          return POP_B();
      case 0xC2:
          return JNZ();
      case 0xC3:
          return JMP();
      case 0xC4:
          return CNZ();
      case 0xC5:
          return PUSH_B();
      case 0xC6:
          return ADI();
      case 0xC7:
          return RST_0();
      case 0xC8:
          return RZ();
      case 0xC9:
          return RET();
      case 0xCA:
          return JZ();
      case 0xCB:
          return JMP();
      case 0xCC:
          return CZ();
      case 0xCD:
          return CALL();
      case 0xCE:
          return ACI();
      case 0xCF:
          return RST_1();
      case 0xD0:
          return RNC();
      case 0xD1:
          return POP_D();
      case 0xD2:
          return JNC();
      case 0xD3:
          return OUT();
      case 0xD4:
          return CNC();
      case 0xD5:
          return PUSH_D();
      case 0xD6:
          return SUI();
      case 0xD7:
          return RST_2();
      case 0xD8:
          return RC();
      case 0xD9:
          return RET();
      case 0xDA:
          return JC();
      case 0xDB:
          return IN();
      case 0xDC:
          return CC();
      case 0xDD:
          return CALL();
      case 0xDE:
          return SBI();
      case 0xDF:
          return RST_3();
      case 0xE0:
          return RPO();
      case 0xE1:
          return POP_H();
      case 0xE2:
          return JPO();
      case 0xE3:
          return XTHL();
      case 0xE4:
          return CPO();
      case 0xE5:
          return PUSH_H();
      case 0xE6:
          return ANI();
      case 0xE7:
          return RST_4();
      case 0xE8:
          return RPE();
      case 0xE9:
          return PCHL();
      case 0xEA:
          return JPE();
      case 0xEB:
          return XCHG();
      case 0xEC:
          return CPE();
      case 0xED:
          return CALL();
      case 0xEE:
          return XRI();
      case 0xEF:
          return RST_5();
      case 0xF0:
          return RP();
      case 0xF1:
          return POP_PSW();
      case 0xF2:
          return JP();
      case 0xF3:
          return DI();
      case 0xF4:
          return CP();
      case 0xF5:
          return PUSH_PSW();
      case 0xF6:
          return ORI();
      case 0xF7:
          return RST_6();
      case 0xF8:
          return RM();
      case 0xF9:
          return SPHL();
      case 0xFA:
          return JM();
      case 0xFB:
          return EI();
      case 0xFC:
          return CM();
      case 0xFD:
          return CALL();
      case 0xFE:
          return CPI();
      case 0xFF:
          return RST_7();
      default:
          qFatal("Instruction not implemented: %02x", op);
    }
    return 0;
}
