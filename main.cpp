#include <QApplication>
#include <QDebug>
#include <stdint.h>

#include <cpu.h>

int main()
{
    CPU cpu;
    cpu.INR_B();
    qDebug() << cpu.registers.B;
}
