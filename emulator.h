#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>
#include <QBitmap>
#include <QWidget>
#include <QDebug>
#include <QThread>
#include "cpu.h"

#define ROM_FILE_PATH "/home/filip/Dropbox/SI2/Disassembler/invaders.rom"

const int SCREEN_WIDTH_BYTES = 32;
const int SCREEN_HEIGHT_BYTES = 28;

const int SCREEN_WIDTH_PIXELS = 256;
const int SCREEN_HEIGHT_PIXELS = 224;

class Emulator : public QThread
{
Q_OBJECT
public:
    explicit Emulator();
    QImage screen;
    QImage rotatedScreen;

private:
    CPU cpu;

    double decode(uint8_t);
    void VRAMtoScreen();

private:
    void run();

signals:
    void screenUpdated(QImage*);

public slots:
    void inputHandler(const int, bool);
};

#endif // EMULATOR_H
