/**************************************************************************************************
 ** File Name: main.cpp
 ** Description: The main function for the emulator.
**************************************************************************************************/
#include <QApplication>

#include "mainwindow.h"
#include "src/gui/gui.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow main;
    main.show();
    return a.exec();
}
