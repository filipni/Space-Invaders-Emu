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

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    QPushButton* startButton;
    QPushButton* stopButton;
    QHBoxLayout* layout;
    QLabel* screen;

    Emulator emu;

public slots:
    void showScreen(QImage const*);
    void startEmulator();
    void stopEmulator();
    void closeEvent(QCloseEvent*);

signals:
    void inputReceived(const int, bool);
};

#endif // GUI_H
