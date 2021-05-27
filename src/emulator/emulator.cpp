#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QBitMap>
#include <QTime>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QSoundEffect>

#include "emulator.h"

QMediaPlayer* music = new QMediaPlayer();

QSoundEffect effect;

// half frame time set to 1/120 of a second in nanoseconds
// 2 interrupts are sent per frame (60fps), helps us match the 8080 cpu clock speed
#define HALF_FRAME 8333333
// the maximum number of cycles per half frame, based on the 8080 cpu clock speed
// of 2 Mhz
#define MAX_CYCLES 16667;



Emulator::Emulator()
{
    effect.setVolume(0.25f);
    originalScreen = QImage(256, 224, QImage::Format_RGB32);
    transform.rotate(-90);
    transform.scale(2, 2);

    connect(&cpu, SIGNAL(writeOnPort3(int)), this, SLOT(playSoundPort3(int)));
    connect(&cpu, SIGNAL(writeOnPort5(int)), this, SLOT(playSoundPort5(int)));
}

// draws the screen by reading bitmap in video ram at address 0x2400
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
    // the video ram bitmap is rotated clockwise, so we need to rotate counterclockwise correct it
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

//handles the key inputs for player controls
void Emulator::inputHandler(const int key, bool pressed){
    uint8_t bitsPlayer1 = 0;
    uint8_t bitsPlayer2 = 0;
    if(key == Qt::Key_A){
        bitsPlayer1 = 1 << 5;               //player 1/2 left bit
    }
    else if(key == Qt::Key_D){
        bitsPlayer1 = 1 << 6;               //player 1/2 right bit
    }
    else if(key == Qt::Key_1){
        bitsPlayer1 = 1 << 2;               //player 1 start
    }
    else if(key == Qt::Key_2){
        bitsPlayer1 = 1 << 1;               //2 player start
        cpu.twoPlayer = true;
    }
    else if(key == Qt::Key_W){
        bitsPlayer1 = 1 << 4;               //player 1/2 fire
    }
    else if(key == Qt::Key_C){
        bitsPlayer1 = 1;                    //coin button
    }
    else if(key == Qt::Key_Right){
        bitsPlayer2 = 1 << 6;               //player 2 right
    }
    else if(key == Qt::Key_Left){
        bitsPlayer2 = 1 << 5;               //player 2 left
    }
    else if(key == Qt::Key_Up){
        bitsPlayer2 = 1 << 4;               //player 2 fire
    }

    if(pressed){
        cpu.input1 = cpu.input1 | bitsPlayer1;
        if(cpu.twoPlayer){
            cpu.input2 = cpu.input2 | bitsPlayer2;
        }
    }
    else{
        cpu.input1 = cpu.input1  & (bitsPlayer1 ^ 0xFF);
        if(cpu.twoPlayer){
            cpu.input2 = cpu.input2  & (bitsPlayer2 ^ 0xFF);
        }
    }

}


/**************************************************************************************************
 * Function Name: resetSound Emulator::playSoundPort3(int raw)
 * Description: Plays the appropriate sound file based on the current sound output
 * opcode sent to port 3.
**************************************************************************************************/

void Emulator::playSoundPort3(int raw){
    // the sound being played is dependent on which bit is set in the a register
    effect.setLoopCount(1);
    if(raw & 1){
        effect.setSource(QUrl("qrc:/sounds/ufo_highpitch.wav"));
        effect.play();
    }
    else if((raw >> 1) & 1){
        effect.setSource(QUrl("qrc:/sounds/shoot.wav"));
        effect.play();

    }
    else if((raw >> 2) & 1 ){
        effect.setSource(QUrl("qrc:/sounds/explosion.wav"));
        effect.play();
    }
    else if((raw >> 3) & 1 ){
        effect.setSource(QUrl("qrc:/sounds/invaderkilled.wav"));
        effect.play();
    }
}

void Emulator::playSoundPort5(int raw){
    // the sound being played is dependent on which bit is set in the a register
    if(raw & 1){
        music->setMedia(QUrl("qrc:/sounds/fastinvader1.wav"));
        resetSound(music);
    }
    else if((raw >> 1) & 1){
        music->setMedia(QUrl("qrc:/sounds/fastinvader2.wav"));
        resetSound(music);
    }
    else if((raw >> 2) & 1){
        music->setMedia(QUrl("qrc:/sounds/fastinvader3.wav"));
        resetSound(music);
    }
    else if((raw >> 3) & 1){
        music->setMedia(QUrl("qrc:/sounds/fastinvader4.wav"));
        resetSound(music);
    }
    else if((raw >> 4) & 1){
        effect.setSource(QUrl("qrc:/sounds/invaderkilled.wav"));
        effect.play();
    }
}


/**************************************************************************************************
 * Function Name: resetSound Emulator::resetSound(QMediaPlayer *sound)
 * Description: This function takes the current sound being played and plays the audio, if the
 * sound has already been played, the starting point is reset to 0 for it to be played from the beginning.
**************************************************************************************************/
void Emulator::resetSound(QMediaPlayer *music){
    if(music->state()== QMediaPlayer::PlayingState){
        music->setPosition(0);
    }
    else if(music->state()== QMediaPlayer::StoppedState){
           music->play();
    }
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

    // start timer
    QElapsedTimer frameTimer;
    // start cycle counter
    int cyclesUntilInterrupt = MAX_CYCLES;
    bool vBlank = true;
    while(true){
        // check if max cycles has been reached
       if(cyclesUntilInterrupt <= 0){
            // if enough cycles have been executed, but not enough time has passed,
            // do not execute any more instructions until enough time has passed
            if(frameTimer.nsecsElapsed() < HALF_FRAME){
                continue;

            }

            // once enough cycles per half frame have been done, and enough time has passed
            // send one of the 2 interrupts, the one that's different from the last interrupt sent
            bool interruptSuccessful;
            if(vBlank){
                interruptSuccessful = cpu.generateInterrupt(0xCF);
            }
            else{
                // When sending VBI, also paint screen, as that means one frame is done being generated in video RAM
                interruptSuccessful = cpu.generateInterrupt(0xD7);
                paintScreen();
            }

            
            if(interruptSuccessful){
                // restart timer, reset cycle counter, and flip VBI tracker on successful interrupt
                vBlank = !vBlank;
                cyclesUntilInterrupt = MAX_CYCLES;
                frameTimer.restart();
            }
        }
        // emulate instructions
        int cycles = cpu.emulateInstruction();
        cyclesUntilInterrupt -= cycles;
    }
}
