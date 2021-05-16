/**************************************************************************************************
 ** File Name: emulator.h
 ** Description: This file contains the Class declaration for the Emulator QWidget class. The
        Emulator will inherit from the QThread class for ease of introducing threading for the
        sound of the Emulator.
**************************************************************************************************/
#include <QWidget>
#include <QThread>
#include <QDebug>
#include <QBitMap>
#include <QtMultimedia/QMediaPlayer>

#include "../cpu/8080State.h"
#include "../cpu/QCpu.h"

#ifndef EMULATOR_H
#define EMULATOR_H


class Emulator : public QThread
{
  Q_OBJECT
public:
    explicit Emulator();
    Qcpu* cpu;
    void run();
private:
    QImage originalScreen;                  //screen prior to rotation
    QImage rotatedScreen;                   //screen the user sees
    QTransform transformScreen;             //transformation factors for the screen

    void paintScreen();                     //paints the screen each frame
    QColor pixelColor(int pixelPosition);   //function for determining specific pixel color
    void resetSound(QMediaPlayer *music);    // Used to control sound setting

signals:
   void screenIsUpdated(const QImage*);     //signal to notify GUI that screen has changed

public slots:
   void inputHandler(const int, bool);      //placeholder slot for keyboard input
   void playSoundPort3(int);                //placeholder slot for playing sound on port 3
   void playSoundPort5(int);                //placeholder slot for playing sound on port 5
};

#endif // EMULATOR_H
