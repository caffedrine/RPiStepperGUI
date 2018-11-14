#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QCoreApplication>

#include <QObject>
#include <QDebug>
#include <QThread>

#include "MainWindow.h"



class MainClass: public QObject
{
    Q_OBJECT
public:
    MainClass();
    ~MainClass();
    void SetUi(MainWindow *_ui);

private:
    MainWindow *ui;

public slots:
    void MainLoop();
    Q_INVOKABLE void onSwitchChanged_Valves(bool checked);
    Q_INVOKABLE void onSwitchChanged_Cutter(bool checked);

    Q_INVOKABLE void onButtonPressed_Connect(QString ip, int port);
    Q_INVOKABLE void onButtonPressed_Reset();
    Q_INVOKABLE void onButtonPressed_Lock();
    Q_INVOKABLE void onButtonPressed_Unlock();
    Q_INVOKABLE void onButtonPressed_Cut();
    Q_INVOKABLE void onButtonPressed_MoveTo(int size);
    Q_INVOKABLE void onButtonPressed_Up();
    Q_INVOKABLE void onButtonPressed_Down();
    Q_INVOKABLE void onButtonPressed_Right();
    Q_INVOKABLE void onbuttonpressed_Left();
    Q_INVOKABLE void onButtonReleased_Up();
    Q_INVOKABLE void onButtonReleased_Down();
    Q_INVOKABLE void onButtonReleased_Right();
    Q_INVOKABLE void onButtonReleased_Left();
};

#endif // MAINCLASS_H
