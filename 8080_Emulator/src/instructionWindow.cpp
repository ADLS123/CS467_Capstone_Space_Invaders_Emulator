#include "instructionWindow.h"
#include "ui_instructionWindow.h"

InstructionWindow::InstructionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstructionWindow)
{
    ui->setupUi(this);
}

InstructionWindow::~InstructionWindow()
{
    delete ui;
}
