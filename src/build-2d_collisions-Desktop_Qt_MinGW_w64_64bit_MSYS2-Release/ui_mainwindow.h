/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *graphicsView;
    QPushButton *startButton;
    QLabel *fps_count;
    QComboBox *algorithm;
    QCheckBox *is_multithread;
    QLabel *label;
    QCheckBox *is_visual;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QPushButton *startButton_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(810, 644);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(10, 0, 600, 600));
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(620, 540, 181, 61));
        fps_count = new QLabel(centralwidget);
        fps_count->setObjectName(QString::fromUtf8("fps_count"));
        fps_count->setGeometry(QRect(720, 504, 81, 20));
        algorithm = new QComboBox(centralwidget);
        algorithm->addItem(QString());
        algorithm->addItem(QString());
        algorithm->addItem(QString());
        algorithm->addItem(QString());
        algorithm->addItem(QString());
        algorithm->addItem(QString());
        algorithm->setObjectName(QString::fromUtf8("algorithm"));
        algorithm->setGeometry(QRect(620, 10, 181, 31));
        is_multithread = new QCheckBox(centralwidget);
        is_multithread->setObjectName(QString::fromUtf8("is_multithread"));
        is_multithread->setGeometry(QRect(620, 50, 181, 21));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(630, 499, 71, 31));
        is_visual = new QCheckBox(centralwidget);
        is_visual->setObjectName(QString::fromUtf8("is_visual"));
        is_visual->setGeometry(QRect(620, 80, 181, 20));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(620, 170, 181, 31));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(620, 140, 181, 31));
        startButton_2 = new QPushButton(centralwidget);
        startButton_2->setObjectName(QString::fromUtf8("startButton_2"));
        startButton_2->setGeometry(QRect(620, 210, 181, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 810, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\240\320\260\320\267\320\261\320\270\320\265\320\275\320\270\320\265 \320\277\321\200\320\276\321\201\321\202\321\200\320\260\320\275\321\201\321\202\320\262\320\260", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\200\321\202", nullptr));
        fps_count->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        algorithm->setItemText(0, QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\320\275\321\213\320\271 \320\277\320\265\321\200\320\265\320\261\320\276\321\200", nullptr));
        algorithm->setItemText(1, QCoreApplication::translate("MainWindow", "\320\224\320\265\321\200\320\265\320\262\320\276 \320\272\320\262\320\260\320\264\321\200\320\260\320\275\321\202\320\276\320\262", nullptr));
        algorithm->setItemText(2, QCoreApplication::translate("MainWindow", "\320\224\320\270\320\275\320\260\320\274\320\270\321\207\320\265\321\201\320\272\320\270\320\271 \321\206\320\265\320\275\321\202\321\200", nullptr));
        algorithm->setItemText(3, QCoreApplication::translate("MainWindow", "9-\320\264\320\265\321\200\320\265\320\262\320\276", nullptr));
        algorithm->setItemText(4, QCoreApplication::translate("MainWindow", "\320\221\320\270\320\275\320\260\321\200\320\275\320\276\320\265 \320\264\320\265\321\200\320\265\320\262\320\276", nullptr));
        algorithm->setItemText(5, QCoreApplication::translate("MainWindow", "\320\220\320\273\320\263\320\276\321\200\320\270\321\202\320\274 \320\272\320\276\321\200\320\267\320\270\320\275", nullptr));

        is_multithread->setText(QCoreApplication::translate("MainWindow", "\320\234\320\275\320\276\320\263\320\276\320\277\320\276\321\202\320\276\321\207\320\275\320\276\321\201\321\202\321\214", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "FPS:", nullptr));
        is_visual->setText(QCoreApplication::translate("MainWindow", "\320\222\320\270\320\267\321\203\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        lineEdit->setText(QCoreApplication::translate("MainWindow", "100", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \321\210\320\260\321\200\320\276\320\262:", nullptr));
        startButton_2->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\264\320\260\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
