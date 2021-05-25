#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QBitMap>
#include <QTime>
#include "emulator.h"

#define HALF_FRAME 9
#define MAX_CYCLES 1000000;

Emulator::Emulator()
{
    originalScreen = QImage(256, 224, QImage::Format_RGB32);
    transform.rotate(-90);
    transform.scale(2, 2);

    connect(&cpu, SIGNAL(writeOnPort3(int)), this, SLOT(playSoundPort3(int)));
    connect(&cpu, SIGNAL(writeOnPort5(int)), this, SLOT(playSoundPort5(int)));
}

void Emulator::paintScreen(){
    for(int i = 0; i < 224; ++i){
        for(int j = 0; j < 32; ++j){
            uint8_t currentByte = cpu.memory[0x2400 + i * 32 + j];
            for(int k = 0; k < 8; ++k){
                int xPosition = j * 8 + k;
                int yPosition = i;

                QColor color;
                int currentPixel = currentByte >> k & 1;
                if(currentPixel){
                    color = paintPixel(xPosition);
                }
                else{
                    color = Qt::black;
                }
                originalScreen.setPixel(xPosition, yPosition, color.rgb());
            }
        }
    }

    rotatedScreen = originalScreen.transformed(transform);
    emit screenIsUpdated(&rotatedScreen);
}


QColor Emulator::paintPixel(int pixelPosition){
    if(pixelPosition >= 224){
        return Qt::white;
    }
    else if(pixelPosition >= 192){
        return Qt::red;
    }
    else if(pixelPosition >= 72){
        return Qt::white;
    }
    else if(pixelPosition >= 16){
        return Qt::green;
    }
    else{
        return Qt::white;
    }
}

//placeholders for signals
void Emulator::inputHandler(const int key, bool pressed){
    uint8_t bits = 0;
    if(key == Qt::Key_A){
        bits = 1 << 5;          //player 1/2 left bit
    }
    else if(key == Qt::Key_D){
        bits = 1 << 6;          //player 1/2 right bit
    }
    else if(key == Qt::Key_Enter){
        bits = 1 << 2;          //player 1 start
    }
    else if(key == Qt::Key_Space){
        bits = 1 << 1;          //2 player start
    }
    else if(key == Qt::Key_W){
        bits = 1 << 4;          //player 1/2 fire
    }
    else if(key == Qt::Key_C){
        bits = 1;               //coin button
    }

    if(pressed){
        cpu.input1 = cpu.input1 | bits;
        cpu.input2 = cpu.input2 | bits;
    }
    else{
        cpu.input1 = cpu.input1  & (bits ^ 0xFF);
        cpu.input2 = cpu.input2  & (bits ^ 0xFF);
    }

}

void Emulator::playSoundPort3(int){
    return;
}

void Emulator::playSoundPort5(int){
    return;
}


void Emulator::run(){

    QFile rom("../qt_gui2/roms/invaders.rom");
    if(!rom.open(QIODevice::ReadOnly)){
        qFatal("Falied to open the rom file");
    }

    //create byte array for the data
    QByteArray romData = rom.readAll();
    Q_ASSERT(romData.size() == 0x2000);

    rom.close();
    for(int i = 0; i < 0x2000; i++){
        cpu.memory[i] = romData.at(i);
    }


    QElapsedTimer frameTimer;
    int cyclesUntilInterrupt = MAX_CYCLES;
    bool vBlank = true;
    while(true){
        int cycles = cpu.emulateInstruction();
        cyclesUntilInterrupt -= cycles;


            if(cyclesUntilInterrupt <= 0){
//                if(frameTimer.elapsed() < HALF_FRAME){
//                    continue;
//                }
                bool interruptSuccessful;
                if(vBlank){
                    interruptSuccessful = cpu.generateInterrupt(0xCF);
                }
                else{
                    interruptSuccessful = cpu.generateInterrupt(0xD7);
                    paintScreen();
                }

                if(interruptSuccessful){
                    vBlank = !vBlank;
                    cyclesUntilInterrupt = MAX_CYCLES;
                }
            }
    }
}
