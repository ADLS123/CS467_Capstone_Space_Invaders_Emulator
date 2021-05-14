#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QColor>
#include <QLabel>
#include <QImage>
#include "src/cpu/opcodeFunctions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    screen = QImage(256, 224, QImage::Format_RGB32);
    emulator = (State8080*)calloc(1, sizeof(State8080));
    emulator->memory = (uint8_t*)calloc(1, 0x10000);
    createEmulatorScreen(emulator);
    ui->label->setPixmap(QPixmap::fromImage(screen));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createEmulatorScreen(State8080* emulator){
    for(int i = 0; i < 224; ++i){
        for(int j = 0; j < 32; ++j){
            uint8_t currentByte = emulator->memory[0x2400 + i * 32 + j];

            for(int k = 0; k < 8; ++k){
                int xPosition = j * 8 + k;
                int yPosition = i;

                QColor color;
                int currentPixel = currentByte >> k & 1;
                if(currentPixel){
                    color = Qt::white;
                }
                else{
                    color = Qt::black;
                }

                this->screen.setPixel(xPosition, yPosition, color.rgb());
            }
        }
    }
}
