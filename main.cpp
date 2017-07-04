#include <QApplication>
#include <QDebug>
#include <cpu.h>

int main(int argc, char** argv)
{
    CPU cpu;
    qDebug() << cpu.addBytes(-10, 3, false, false);
    //cpu.DCR_A();
    qDebug() << cpu.registers.A;
    return 0;
}
