/**************************************************************************************************
 ** File Name: QCpu.h
 ** Description: Contains the Class declaration for the Qcpu class, which is one of the three
        main QWidgets that make up the Emulator.
**************************************************************************************************/
#include <QObject>

#include <QDebug>

#include "8080State.h"
#include "opcodeFunctions.h"
#include "../machine/ioPorts.h"
#ifndef QCPU_H
#define QCPU_H

class Qcpu: public QObject{
    Q_OBJECT
private:
    struct StateSIMachine* ports;

public:
    Qcpu();
    struct State8080* registers;    //put this in public so there was no need for getters
    void machineIN();               //function for reading inputs
    void machineOUT();              //function for writing to output
    int emulate8080(State8080* state);
signals:
    void writeOnPort3(int);         //signal for the write to output port 3
    void writeOnPort5(int);         //signal for the write to output port 5
};

#endif // QCPU_H
