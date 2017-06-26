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

int CPU::addBytes(int8_t byte1, int8_t byte2, bool carryIn, uint8_t flags)
{
    uint8_t carry = carryIn ? conditionBits.testBit(CARRY_BIT) : 0;

    int8_t sum = 0;
    for (int i = 0; i <= 7; ++i)
    {
        int8_t term1 = byte1 & 1;
        int8_t term2 = byte2 & 1;

        sum += (term1 ^ term2 ^ carry) << i;
        carry = term1 + term2 + carry > 1;

        if (i == 3 && flags & AUX_BIT)
            conditionBits.setBits(AUX_BIT, carry); // Carry from lower to higher nibble

        byte1 >>= 1;
        byte2 >>= 1;
    }

    Q_ASSERT(carry == 1 || carry == 0);

    // Set the rest of the flags
    if (flags & CARRY_BIT) conditionBits.setBits(CARRY_BIT, carry);
    if (flags & ZERO_BIT) conditionBits.setBits(ZERO_BIT, sum == 0);
    if (flags & SIGN_BIT) conditionBits.setBits(SIGN_BIT, (sum & SIGN_BIT) != 0);
    if (flags & PARITY_BIT) conditionBits.setBits(PARITY_BIT, calculateParity(sum) % 2 == 0);

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
    reg = addBytes(reg, 1, false, SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
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
