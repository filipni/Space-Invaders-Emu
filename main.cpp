#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

#include "gui.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    GUI window;
    window.show();

    return app.exec();
}
