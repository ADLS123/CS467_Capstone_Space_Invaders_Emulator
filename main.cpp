/**************************************************************************************************
 ** File Name: main.cpp
 ** Description: Temporary main file to try to get video emulation to work
***************************************************************************************************/
#include "src/gui/gui.h"
#include "src/cpu/QCpu.h"
#include <QObject>
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gui window;
    qDebug() << "Hi from the main function\n";
    window.show();
    return a.exec();


}
