/**************************************************************************************************
 ** File Name: gui.cpp
 ** Description: This file contains the member function definitions for the Gui class.
**************************************************************************************************/
#include "gui.h"

Gui::Gui()
{
    //creates a basic window for the emulator screen
    layout = new QHBoxLayout(this);
    layout->setMargin(0);
    screen = new QLabel(this);
    this->setWindowTitle(QString("Space Invaders"));
    layout->addWidget(screen);
    //connects the emulator signal to the gui slot and input received signal to the handler slot
    connect(&emulator, SIGNAL(screenIsUpdated(const QImage*)), this, SLOT(showScreen(const QImage*)));
    connect(this, SIGNAL(inputReceived(int,bool)), &emulator, SLOT(inputHandler(int,bool)));
    //actually runs the emulator
    emulator.start();
}

//sets the pixelmap for painting the screen
void Gui::showScreen(const QImage* image){
    screen->setPixmap(QPixmap::fromImage(*image));
}

//emits the input received signal for when the control keys are pressed
void Gui::keyPressEvent(QKeyEvent* event){
    emit inputReceived(event->key(), true);
}

//emits the input received signal upon control key release
void Gui::keyReleaseEvent(QKeyEvent* event){
    emit inputReceived(event->key(), false);
}

//destructor that terminates qThread process
Gui::~Gui(){
    emulator.terminate();
    emulator.wait();
}
