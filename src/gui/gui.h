
#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QKeyEvent>

#include "../emulator/emulator.h"

#ifndef GUI_H
#define GUI_H


class Gui: public QWidget{
    Q_OBJECT
public:
    Gui();
private:
    QPushButton* startButton;
    QHBoxLayout* layout;
    QLabel* screen;

    Emulator emulator;

public slots:
    void showScreen(QImage const*);
    void closeEvent(QCloseEvent*);

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
signals:
    void inputReceived(int, bool);
};

#endif // GUI_H
