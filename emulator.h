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

const int SCALE_FACTOR = 3;

const int UPPER_SCREEN = 224;
const int UPPER_MIDDLE_SCREEN = 192;
const int MIDDLE_SCREEN = 72;
const int LOWER_MIDDLE_SCREEN = 16;

const int INTERRUPT_FREQ = 1700000;

class Emulator : public QThread
{
Q_OBJECT
public:
    explicit Emulator();

private:
    CPU cpu;

    QImage originalScreen;
    QImage transformedScreen;
    QTransform transformation;

    void VRAMtoScreen();
    QColor chooseColor(int);

private:
    void run();

signals:
    void screenUpdated(QImage const *);

public slots:
    void inputHandler(const int, bool);
};

#endif // EMULATOR_H
