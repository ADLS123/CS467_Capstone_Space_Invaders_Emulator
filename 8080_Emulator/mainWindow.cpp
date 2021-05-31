/**************************************************************************************************
 ** File Name: mainwindow.cpp
 ** Description: The member function definitions for the MainWindow class
**************************************************************************************************/
#include <QApplication>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

void MainWindow::on_runButton_clicked()
{
    this->hide();       //closes main window
    window.runEmulator();
    window.show();      //opens the actual emulator
}

void MainWindow::on_instructionsButton_clicked()
{
    instructions.show();
}


MainWindow::~MainWindow()
{
    delete ui;
}

