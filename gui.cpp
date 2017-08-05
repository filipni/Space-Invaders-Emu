#include "gui.h"

GUI::GUI()
{
    layout = new QHBoxLayout(this);
    layout->setMargin(0);

    screen = new QLabel(this);
    layout->addWidget(screen);

    connect(&emu, SIGNAL(screenUpdated(QImage const*)), this, SLOT(showScreen(QImage const*)));
    connect(this, SIGNAL(inputReceived(const int, bool)), &emu, SLOT(inputHandler(int, bool)));

    emu.start();
}

void GUI::showScreen(QImage const* image)
{
    screen->setPixmap(QPixmap::fromImage(*image));
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
