/**************************************************************************************************
 ** File name: gui.h
 ** Description: Contains the Class declaration for the Gui class, which will operate the screen
        for the Space Invaders Emulator.
**************************************************************************************************/
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>
#include <QKeyEvent>

#include "../emulator/emulator.h"

#ifndef GUI_H
#define GUI_H


class Gui: public QWidget{
    Q_OBJECT
public:
    Gui();                                  //constructor
    ~Gui();                                 //destructor
private:
    //sets up the Screen for Space Invaders
    QHBoxLayout* layout;
    QLabel* screen;
    Emulator emulator;

public slots:
    void showScreen(QImage const*);         //catches the screen updated signal from Emulator

protected:
    void keyPressEvent(QKeyEvent *);        //catches the QKey event for key presses
    void keyReleaseEvent(QKeyEvent *);      //catches the QKey event for key release
signals:
    void inputReceived(int, bool);
};

#endif // GUI_H
