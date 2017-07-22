#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>
#include <QBitmap>
#include <QWidget>
#include <QDebug>
#include "cpu.h"

#define ROM_FILE_PATH "/home/filip/Dropbox/SI2/Disassembler/invaders.rom"
const int SCREEN_WIDTH = 28;
const int SCREEN_HEIGHT = 32;

class Emulator : public QWidget
{
Q_OBJECT
public:
    Emulator(QWidget* parent = 0);
    void run();
    int decode(uint8_t);

    QImage screen;
    CPU cpu;

    void VRAMtoScreen();

};

#endif // EMULATOR_H
