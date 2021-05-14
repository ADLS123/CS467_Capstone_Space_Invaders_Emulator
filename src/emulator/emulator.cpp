/**************************************************************************************************
 ** File Name: emulator.cpp
 ** Description: This file contains the member function definitions for the Emulator QWidget
        class.
**************************************************************************************************/

#include <QFile>
#include <QTextStream>

#include "emulator.h"

#include "../cpu/opcodeFunctions.h"
#include "../cpu/8080State.h"
#include "../machine/ioPorts.h"

Emulator::Emulator(){

    originalScreen = QImage(256, 224, QImage::Format_RGB32);
    transformScreen.rotate(-90);
    transformScreen.scale(2.0, 2.0);
    cpu = new Qcpu;

    connect(cpu, SIGNAL(writeOnPort3(int)), this, SLOT(playSoundPort3(int)));
    connect(cpu, SIGNAL(writeOnPort5(int)), this,SLOT(playSoundPort5(int)));

}

/**************************************************************************************************
 * Function Name: void Emulator::paintScreen()
 * Description: This function handles the painting of the screen from the video ram.
**************************************************************************************************/
void Emulator::paintScreen(){
    for(int i = 0; i < 224; i++){
        for(int j = 0; j < 32; j++){
            uint8_t currentByte = cpu->registers->memory[0x2400 + i * 32 + j];  //sets current mem byte
            for(int k = 0; k < 8; k++){
                int xPosition = j * 8 + k;
                int yPosition = i;

                QColor color;
                int currentPixel = currentByte >> k & 1;
                if(currentPixel){
                    color = pixelColor(xPosition);              //calls function to color activated pixel
                }
                else{
                    color = Qt::black;
                }
                originalScreen.setPixel(xPosition, yPosition, color.rgb());
            }
        }
    }

    rotatedScreen = originalScreen.transformed(transformScreen);    //updates the screen
    emit screenIsUpdated(&rotatedScreen);                           //calls signal to GUI
}


/**************************************************************************************************
 * Function Name: QColor Emulator::pixelColor(int pixelPosition)
 * Description: This function takes the position of an activated pixel and colors it the correct
    color based on it's position on the emulator screen
**************************************************************************************************/
QColor Emulator::pixelColor(int pixelPosition){
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

//Placeholder for the inputHandler signal
void Emulator::inputHandler(const int key, bool pressed){
    return;
}

//Placeholder for playing sound on port 3
void Emulator::playSoundPort3(int){
    return;
}

//Placeholder for playing sound on port 5
void Emulator::playSoundPort5(int){
    return;
}


/**************************************************************************************************
 * Function Name: void Emulator::runEmulator()
 * Description: This function is responsible for reading the ROM and running the emulator
**************************************************************************************************/
void Emulator::runEmulator(){

    QTextStream out(stdout);
    //NOTE: placeholder reading function until our function is fixed for Qt
    QFile rom("../../Qt_GUI/roms/invaders");
    if(!rom.open(QIODevice::ReadOnly)){
        qFatal("Failed to open rom file\n");
    }
    else{
        out << "Opened invaders rom\n";
    }
    //creates Byte array for the data and reads it into the the memory of the cpu
    QByteArray romData = rom.readAll();

    for(int i = 0; i < 0x2000; i++){
        cpu->registers->memory[i] = romData[i];
    }


    //Placeholder function for tracking time until our timing function can be fixed
    int cyclesUntilInterrupt = 1000000;
    bool vBlankOn = true;
    while (1) {

        uint8_t* opCode = &cpu->registers->memory[cpu->registers->pc];
        int cycles = *opCode;
        cyclesUntilInterrupt -= cycles;
        printf("%d\n", cyclesUntilInterrupt);
        if (*opCode == 0xdb) {	// machine IN
            cpu->machineIN();
            extractOpCode(cpu->registers->memory, cpu->registers->pc);
            cpu->registers->pc += 2;
        }
        else if (*opCode == 0xd3) { // machine OUT
            cpu->machineOUT();
            extractOpCode(cpu->registers->memory, cpu->registers->pc);
            cpu->registers->pc += 2;
        }
        else { // regular 8080 opcodes
            emulate8080(cpu->registers);
            extractOpCode(cpu->registers->memory, cpu->registers->pc);
            //printf("OP Current: %x\nPC: %x\nSP: %x\n", cpu->memory[cpu->pc], cpu->pc, cpu->sp);
            //printf("Registers:  A: %x B: %x C: %x D: %x E: %x H: %x L: %x \n", cpu->a, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l);
            printf("---------------------\n");
        }

        if(cyclesUntilInterrupt <= 0 && cpu->registers->int_enable){
            if(vBlankOn){
                generateInterrupt(cpu->registers, 1);
            }
            else{
                generateInterrupt(cpu->registers, 2);
                paintScreen();
            }

            vBlankOn = !vBlankOn;
           cyclesUntilInterrupt = 1000000;
        }
    }


}
