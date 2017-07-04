#include "cpu.h"

CPU::CPU()
{
    memset(&registers, 0, sizeof(registers));
}


int CPU::addBytes(int8_t byte1, int8_t byte2, bool carryIn, FlagRegister flagsToCalc)
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

void CPU::INR(int8_t &reg)
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

void CPU::DCR(int8_t &reg)
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

/*
void CPU::MOV_B_M()
{
    registers.B =
}
*/

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

/*
void CPU::MOV_C_M()
{
    registers.C =
}
*/

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

/*
void CPU::MOV_D_M()
{
    registers.D =
}
*/

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

/*
void CPU::MOV_E_M()
{
    registers.E =
}
*/

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

/*
void CPU::MOV_H_M()
{
    registers.H = registers.E;
}
*/

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

/*
void CPU::MOV_L_M()
{
    registers.L =
}
*/

void CPU::MOV_L_A()
{
    registers.L = registers.A;
}

/*
void CPU::MOV_M()
{
}
*/

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

/*
void CPU::MOV_A_M()
{
    registers.A =
}
*/

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

void CPU::ADD(int8_t operand)
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

void CPU::ADC(int8_t operand)
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

void CPU::SUB(int8_t operand)
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

void CPU::SBB(int8_t operand)
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

void CPU::ANA(int8_t operand)
{
    registers.A = registers.A & operand;
    conditionBits.setBits(CARRY_BIT, false);
    conditionBits.calculateZeroSignParityBits(registers.A);
}
