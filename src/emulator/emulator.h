#include <QWidget>
#include <QThread>
#include <QBitMap>

#include "../cpu/cpu.h"

#ifndef EMULATOR_H
#define EMULATOR_H


class Emulator : public QThread
{
    Q_OBJECT
public:
    Emulator();
private:
    Cpu cpu;

    QImage originalScreen;
    QImage rotatedScreen;
    QTransform transform;

    void paintScreen();
    QColor paintPixel(int pixelPosition);

    void run();

signals:
    void screenIsUpdated(QImage const*);

public slots:
    void inputHandler(const int, bool);
    void playSoundPort3(int);
    void playSoundPort5(int);
};

#endif // EMULATOR_H
