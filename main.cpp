#include <QApplication>
#include <QDebug>
#include <stdint.h>
#include <stdio.h>
#include <cpu.h>

int main()
{
    CPU cpu;
    qDebug() << cpu.addBytes(-10, 3, false, false);
    //cpu.DCR_A();
    qDebug() << cpu.registers.A;
}
