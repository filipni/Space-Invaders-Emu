#include "gui.h"

GUI::GUI()
{
    layout = new QHBoxLayout(this);

    screen = new QLabel(this);
    layout->addWidget(screen);

    startButton = new QPushButton("Start", this);
    layout->addWidget(startButton);
    connect(startButton, SIGNAL(pressed()), this, SLOT(startEmulator()));

    stopButton = new QPushButton("Stop", this);
    layout->addWidget(stopButton);
    stopButton->setEnabled(false);
    connect(stopButton, SIGNAL(pressed()), this, SLOT(stopEmulator()));

    connect(&emu, SIGNAL(screenUpdated(QImage const*)), this, SLOT(showScreen(QImage const*)));
    connect(this, SIGNAL(inputReceived(const int, bool)), &emu, SLOT(inputHandler(int, bool)));
}

void GUI::showScreen(QImage const* image)
{
    screen->setPixmap(QPixmap::fromImage(*image));
}

void GUI::startEmulator()
{
    emu.start();
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
}

void GUI::stopEmulator()
{
    emu.terminate();
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}

void GUI::closeEvent(QCloseEvent*)
{
    emu.terminate();
    emu.wait();
}

void GUI::keyPressEvent(QKeyEvent* event)
{
    emit inputReceived(event->key(), true);
}

void GUI::keyReleaseEvent(QKeyEvent* event)
{
    emit inputReceived(event->key(), false);
}
