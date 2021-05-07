#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "helperFunctions.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createWindowTitle(){
    this->setWindowTitle("Space Invaders");
}

void MainWindow::on_pushButton_clicked()
{
    runRom();
}
