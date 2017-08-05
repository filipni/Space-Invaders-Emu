#include "emulator.h"
#include <QFile>
#include <QTextStream>
#include <QColor>
#include <QDebug>

QTextStream out(stdout);

Emulator::Emulator()
{
    originalScreen = QImage(SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS, QImage::Format_RGB32);

    // This transformation will be applied before showing the screen
    transformation.rotate(-90);
    transformation.scale(SCALE_FACTOR, SCALE_FACTOR);
}

void Emulator::VRAMtoScreen()
{
    for (int i = 0; i < SCREEN_HEIGHT_PIXELS; ++i)
    {
        for (int j = 0; j < SCREEN_WIDTH_BYTES; ++j)
        {
            uint8_t currentByte = cpu.memory[VIDEO_RAM_START + i * SCREEN_WIDTH_BYTES + j];

            for (int k = 0; k < 8; ++k)
            {
                int xPos = j * 8 + k;
                int yPos = i;

                QColor color;
                int currentPixel = currentByte >> k & 1;
                if (currentPixel)
                  color = chooseColor(xPos);
                else
                  color = Qt::black;

                originalScreen.setPixel(xPos, yPos, color.rgb());
            }
        }
    }

    transformedScreen = originalScreen.transformed(transformation);
    emit screenUpdated(&transformedScreen);
}

QColor Emulator::chooseColor(int y)
{
    if (y >= UPPER_SCREEN)
        return Qt::white;
    else if (y >= UPPER_MIDDLE_SCREEN)
        return Qt::red;
    else if (y >= MIDDLE_SCREEN)
        return Qt::white;
    else if (y >= LOWER_MIDDLE_SCREEN)
        return Qt::green;
    else
        return Qt::white;
}

void Emulator::inputHandler(const int key, bool pressed)
{
    uint8_t bitmask = 0;
    if (key == Qt::Key_Left)
        bitmask = P1_LEFT;
    else if (key == Qt::Key_Right)
        bitmask = P1_RIGHT;
    else if (key == Qt::Key_Return)
        bitmask = P1_START;
    else if (key == Qt::Key_Space)
        bitmask = P1_SHOOT;
    else if (key == Qt::Key_C)
        bitmask = COIN;

    if (pressed)
        cpu.input1 |= bitmask;
    else
        cpu.input1 &= bitmask ^ 0xFF;
}

void Emulator::run()
{
    QFile romFile(ROM_FILE_PATH);

    if (!romFile.open(QIODevice::ReadOnly))
        qFatal("Could not open rom file.");
    else
       out << "Opened " + QString(ROM_FILE_PATH) << endl;

    QByteArray fileData = romFile.readAll();
    Q_ASSERT(fileData.size() == ROM_SIZE);

    for (int i = ROM_START; i < ROM_SIZE; ++i)
        cpu.memory[i] = fileData.at(i);

    int cyclesTillEvent = INTERRUPT_FREQ;
    bool vblank = true;
    while (true)
    {
        int cycles = cpu.runNextInstruction();
        cyclesTillEvent -= cycles;

        if (cyclesTillEvent <= 0)
        {
            bool interruptSuccess;
            if (vblank)
                interruptSuccess = cpu.generateInterrupt(RST_1_OPCODE);
            else
            {
                interruptSuccess = cpu.generateInterrupt(RST_2_OPCODE);
                VRAMtoScreen();
            }

            if (interruptSuccess)
            {
                vblank = !vblank;
                cyclesTillEvent = INTERRUPT_FREQ;
            }
        }
    }
}
