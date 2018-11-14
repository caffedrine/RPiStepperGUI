#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QCoreApplication>

#include <QObject>
#include <QDebug>
#include <QThread>

#include "MainWindow.h"

enum class StatusType
{
    SUCCESS = 0,
    ERROR = 1,
    PENDING = 2
};

class MainClass: public QObject
{
    Q_OBJECT
public:
    MainClass(MainWindow *_ui);
    ~MainClass();
    void SetStatus(QString text, StatusType status);

private:
    MainWindow *ui;

public slots:
    void MainLoop();
};

#endif // MAINCLASS_H
