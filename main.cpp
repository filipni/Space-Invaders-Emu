#include <QApplication>
#include <QDebug>
#include <stdint.h>
#include <stdio.h>
#include <cpu.h>

int main()
{
    CPU cpu;
    //cpu.addBytes(-5, -5, false, false);
    cpu.DCR_A();
    qDebug() << cpu.registers.A;
}
