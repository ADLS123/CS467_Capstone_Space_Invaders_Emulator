/**************************************************************************************************
 ** File Name: gui.cpp
 ** Description: Temporary gui file to get a screen running for the emulator. Will be changed to
        work with current GUI design once bugs are fixed
**************************************************************************************************/

#include "gui.h"

Gui::Gui(){
    layout = new QHBoxLayout(this);
    layout->setMargin(0);
    screen = new QLabel(this);
    emulator = new Emulator;
    layout->addWidget(screen);
    setMinimumSize(200,200);
    connect(emulator, SIGNAL(screenIsUpdated(const QImage*)), this, SLOT(showScreen(const QImage*)));
    connect(this, SIGNAL(inputReceived(int,bool)), emulator, SLOT(inputHandler(int,bool)));

    emulator->start();

}


void Gui::showScreen(const QImage* image){
    screen->setPixmap(QPixmap::fromImage(*image));
}

void Gui::closeEvent(QCloseEvent*){
    emulator->terminate();
    emulator->wait();
}
