#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

#include "emulator.h"

class GUI : public QWidget
{
Q_OBJECT
public:
    GUI();

private:
    QPushButton* startButton;
    QPushButton* stopButton;
    QHBoxLayout* layout;
    QLabel* screen;

    Emulator emu;

public slots:
    void showScreen(QImage*);
    void startEmulator();
    void stopEmulator();
    void closeEvent(QCloseEvent*);
};

#endif // GUI_H
