#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>
#include <QBitmap>
#include <QWidget>
#include <QDebug>
#include <QThread>
#include "cpu.h"

#define ROM_FILE_PATH ":/roms/invaders"

// Sound files
#define INVADER_1_SFX ":sfx/fastinvader1"
#define INVADER_2_SFX ":sfx/fastinvader2"
#define INVADER_3_SFX ":sfx/fastinvader3"
#define INVADER_4_SFX ":sfx/fastinvader4"
#define INVADER_DIES_SFX ":sfx/invaderkilled"

#define PLAYER_SHOOTING_SFX ":sfx/shoot"
#define PLAYER_DIES_SFX ":sfx/explosion"

#define UFO_SFX ":sfx/ufo_highpitch"
#define UFO_DIES_SFX ":sfx/ufo_lowpitch"

const int SCREEN_WIDTH_BYTES = 32;
const int SCREEN_HEIGHT_BYTES = 28;

const int SCREEN_WIDTH_PIXELS = 256;
const int SCREEN_HEIGHT_PIXELS = 224;

const int SCREEN_SCALE_FACTOR = 3;

const int UPPER_SCREEN = 224;
const int UPPER_MIDDLE_SCREEN = 192;
const int MIDDLE_SCREEN = 72;
const int LOWER_MIDDLE_SCREEN = 16;

const int INTERRUPT_FREQ = 1000000;

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
    void playSoundPort3(int);
    void playSoundPort5(int);
};

#endif // EMULATOR_H
