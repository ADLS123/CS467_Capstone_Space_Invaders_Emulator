/**************************************************************************************************
 ** File Name: gui.h
 ** Description: The class declaration for the GUI QWidget class, this is the third and final
        main component of the Emulator, responsible for displaying the emulator

 ** NOTE: (From Devin) This is a placeholder that creates a new window in main.cpp for the gui,
        after video functions I'll work on putting it to the existing GUI in the mainwindow file
**************************************************************************************************/
#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

#include "../emulator/emulator.h"

#ifndef GUI_H
#define GUI_H

class Gui : public QWidget{
    Q_OBJECT
public:
    Gui();
private:
    QPushButton* startButton;
    QHBoxLayout* layout;
    QLabel* screen;

    Emulator* emulator;

public slots:
    void showScreen(QImage const*);         //slot for showing the screen as it updates
    void closeEvent(QCloseEvent*);          //close event for closing the emulator

signals:
    void inputReceived(int,bool);           //placeholder signal for receiving input
};

#endif // GUI_H
