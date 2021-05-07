#include "mainwindow.h"
#include "disassembler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.createWindowTitle();
    w.show();
    return a.exec();
}
