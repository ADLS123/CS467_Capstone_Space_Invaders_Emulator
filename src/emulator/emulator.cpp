#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QBitMap>
#include <QTime>
#include "emulator.h"
#include <QtMultimedia/QMediaPlayer>
#include <QSoundEffect>


QMediaPlayer *music = new QMediaPlayer();
QSoundEffect effect;


#define HALF_FRAME 9
#define MAX_CYCLES 1000000;

Emulator::Emulator()
{
    effect.setVolume(0.25f);

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
effect.setLoopCount(1);
    if(raw & 0){
        music->setMedia(QUrl("qrc:/sounds/ufo_highpitch.wav"));
        resetSound(music);
    }

     else if((raw >> 1) & 1 && (effect.isPlaying() == false)){
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
/**************************************************************************************************
 * Function Name: resetSound Emulator::playSoundPort5(int raw)
 * Description: Plays the appropriate sound file based on the current sound output
 * opcode sent to port 5.
**************************************************************************************************/
void Emulator::playSoundPort5(int raw){



    if(raw & 0){
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
    else if((raw >> 4) & 1  && (effect.isPlaying() == false)){

        effect.setSource(QUrl("qrc:/sounds/invaderkilled.wav"));
        effect.play();
    }

}

/**************************************************************************************************
 * Function Name: resetSound Emulator::resetSound(QMediaPlayer *sound)
 * Description: This function takes the current sound being played and plays the audio, if the
 * sound has already been played, the starting point is reset to 0 for it to be played from the beginning.
**************************************************************************************************/
void Emulator::resetSound(QMediaPlayer *sound){
    if(sound->state()== QMediaPlayer::PlayingState){
        sound->setPosition(0);
    }
    else if(sound->state()== QMediaPlayer::StoppedState){
           sound->play();
    }
}


void Emulator::run(){

    QFile rom("../Qt_GUI/roms/invaders.rom");
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
