#include "cpu.h"

CPU::CPU()
{
    flags =
    {
        .aux 		= false,
        .sign 		= false,
        .zero		= false,
        .parity		= false,
        .carry 		= false
    };

    registers =
    {
        .A 			= 0,
        .B			= 0,
        .C			= 0,
        .D			= 0,
        .E			= 0,
        .H			= 0,
        .L			= 0
    };
}

int8_t CPU::calculateParity(int8_t reg)
{
   int8_t count = 0;
   while (reg)
   {
       if ((reg & 1) == 1)
           ++count;
       reg >>= 1;
   }
   return count;
}

int CPU::addBytes(int8_t byte1, int8_t byte2, bool carryIn, bool carryOut)
{
    int8_t carry = carryIn ? flags.carry : 0;

    int8_t sum = 0;
    for (int i = 0; i <= 7; ++i)
    {
        int8_t term1 = byte1 & 1;
        int8_t term2 = byte2 & 1;

        sum += (term1 ^ term2 ^ carry) << i;
        carry = term1 + term2 + carry > 1;

        if (i == 3)
            flags.aux = carry; // Carry from lower to higher nibble

        byte1 >>= 1;
        byte2 >>= 1;
    }

    // Set the rest of the flags
    if (carryOut) flags.carry = carry;
    flags.zero = sum == 0;
    flags.sign = (sum & SIGN_BIT) != 0;
    flags.parity = calculateParity(sum) % 2 == 0;

    return sum;
}

void CPU::NOP() {}

void CPU::CMC()
{
   flags.carry = !flags.carry;
}

void CPU::STC()
{
    flags.carry = true;
}

void CPU::INR(int8_t &reg)
{
    reg = addBytes(reg, 1, false, false);
}

void CPU::INR_A() { INR(registers.A); }
void CPU::INR_B() { INR(registers.B); }
void CPU::INR_C() { INR(registers.C); }
void CPU::INR_D() { INR(registers.D); }
void CPU::INR_E() { INR(registers.E); }
void CPU::INR_H() { INR(registers.H); }
void CPU::INR_L() { INR(registers.L); }
//void CPU::INR_M() { INR(registers.M); }
