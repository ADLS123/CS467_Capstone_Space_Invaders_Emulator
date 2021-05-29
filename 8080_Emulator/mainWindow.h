#include <QMainWindow>

#include "instructionWindow.h"
#include "src/gui/gui.h"


#ifndef MAINWINDOW_H
#define MAINWINDOW_H



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    Gui window;                                 //gui for emulation
    InstructionWindow instructions;             //instructions window
    ~MainWindow();

public slots:
    void on_runButton_clicked();                //slot for the run button being clicked

private slots:
    void on_instructionsButton_clicked();       //slot for instructions button being clicked

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
