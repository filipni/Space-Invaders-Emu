#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>
#include <QBitmap>
#include <QWidget>
#include <QDebug>
#include "cpu.h"

#define ROM_FILE_PATH "/home/filip/Dropbox/SI2/Disassembler/invaders.rom"

const int SCREEN_WIDTH_BYTES = 32;
const int SCREEN_HEIGHT_BYTES = 28;

const int SCREEN_WIDTH_PIXELS = 256;
const int SCREEN_HEIGHT_PIXELS = 224;

class Emulator : public QWidget
{
Q_OBJECT
public:
    Emulator(QWidget* parent = 0);
    void run();
    double decode(uint8_t);

    QImage screen;
    CPU cpu;

    void VRAMtoScreen();

};

#endif // EMULATOR_H
