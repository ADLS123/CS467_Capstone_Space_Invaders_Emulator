/**************************************************************************************************
    ** File Name: emulator.h
    ** Description: This file contains the Class declaration for the Emulator class.
**************************************************************************************************/
#include <QWidget>
#include <QThread>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QSoundEffect>

#include "../cpu/cpu.h"

#ifndef EMULATOR_H
#define EMULATOR_H


class Emulator : public QThread
{
    Q_OBJECT
public:
    Emulator();                             //constructor
private:
    Cpu cpu;

    QImage originalScreen;                  //screen in its original form
    QImage rotatedScreen;                   //screen displayed to the user
    QTransform transform;                   //transformation factor for the screen

    void paintScreen();
    QColor paintPixel(int pixelPosition);
    void resetSound();                      // Used to control sound setting

    void run();

signals:
    void screenIsUpdated(QImage const*);    //signal sent to gui that screen changed

public slots:
    void inputHandler(const int key, bool pressed);
    void playSoundPort3(int);
    void playSoundPort5(int);
};

#endif // EMULATOR_H
