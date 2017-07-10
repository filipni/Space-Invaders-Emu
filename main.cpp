#include <QApplication>
#include <QDebug>
#include "cpu.h"
#include "emulator.h"

int main(int argc, char** argv)
{
    Emulator emu;
    emu.run();
    return 0;
}
