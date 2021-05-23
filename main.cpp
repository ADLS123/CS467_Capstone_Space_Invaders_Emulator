#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

#include "src/gui/gui.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gui window;
    window.show();
    return a.exec();
}
