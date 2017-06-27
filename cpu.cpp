#include "cpu.h"

CPU::CPU()
{
    memset(&registers, 0, sizeof(registers));
}

uint8_t CPU::calculateParity(uint8_t reg) // Register needs to be converted to unsigned for method to work
{
   uint8_t count = 0;
   while (reg)
   {
       if ((reg & 1) == 1)
           ++count;
       reg >>= 1;
   }
   return count;
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
    if (flagsToCalc.testBits(ZERO_BIT)) conditionBits.setBits(ZERO_BIT, sum == 0);
    if (flagsToCalc.testBits(SIGN_BIT)) conditionBits.setBits(SIGN_BIT, (sum & SIGN_BIT) != 0);
    if (flagsToCalc.testBits(PARITY_BIT)) conditionBits.setBits(PARITY_BIT, calculateParity(sum) % 2 == 0);

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
