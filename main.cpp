#include <QApplication>
#include <QDebug>
#include <QLabel>
#include "cpu.h"
#include "emulator.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Emulator emu;
    emu.run();
    //emu.show();
    return app.exec();
}
