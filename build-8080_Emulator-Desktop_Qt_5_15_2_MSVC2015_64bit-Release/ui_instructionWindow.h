/********************************************************************************
** Form generated from reading UI file 'instructionWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSTRUCTIONWINDOW_H
#define UI_INSTRUCTIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InstructionWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *InstructionWindow)
    {
        if (InstructionWindow->objectName().isEmpty())
            InstructionWindow->setObjectName(QString::fromUtf8("InstructionWindow"));
        InstructionWindow->resize(600, 400);
        InstructionWindow->setMinimumSize(QSize(600, 400));
        InstructionWindow->setMaximumSize(QSize(600, 400));
        InstructionWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        centralwidget = new QWidget(InstructionWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 10, 251, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("Fixedsys"));
        font.setPointSize(38);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 70, 431, 91));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Fixedsys"));
        font1.setPointSize(22);
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_2->setFrameShape(QFrame::NoFrame);
        label_2->setWordWrap(true);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 180, 251, 161));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Fixedsys"));
        label_3->setFont(font2);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(340, 170, 231, 161));
        label_4->setFont(font2);
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        InstructionWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(InstructionWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 600, 20));
        InstructionWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(InstructionWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        InstructionWindow->setStatusBar(statusbar);

        retranslateUi(InstructionWindow);

        QMetaObject::connectSlotsByName(InstructionWindow);
    } // setupUi

    void retranslateUi(QMainWindow *InstructionWindow)
    {
        InstructionWindow->setWindowTitle(QCoreApplication::translate("InstructionWindow", "Instructions", nullptr));
        label->setText(QCoreApplication::translate("InstructionWindow", "Instructions ", nullptr));
        label_2->setText(QCoreApplication::translate("InstructionWindow", "Goal: \n"
" Destroy all of the Space Invaders to get the highest score you can. Invaders will keep coming until you lose all lives, all of the bases get destroyed, or the invaders reach the ground.", nullptr));
        label_3->setText(QCoreApplication::translate("InstructionWindow", "Main Menu Controls:\n"
"Insert Coin: 'C'\n"
"*For 2 Players press 'C' twice*\n"
"\n"
"Player 1 Start: Press 1\n"
"Player 2 Start: Press 2\n"
"", nullptr));
        label_4->setText(QCoreApplication::translate("InstructionWindow", "Player 1 Controls:\n"
"Move Left: 'A'\n"
"Move Right: 'D'\n"
"Fire: 'W'\n"
"\n"
"Player 2 Controls:\n"
"Move Left: Left Key\n"
"Move Right: Right Key\n"
"Fire: Up Key\n"
"", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InstructionWindow: public Ui_InstructionWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSTRUCTIONWINDOW_H
