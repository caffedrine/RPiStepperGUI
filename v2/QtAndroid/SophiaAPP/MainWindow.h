#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QQmlContext>

#include "LockHelper.h"



class MainWindow : public QObject
{
    Q_OBJECT
public:
    QQmlApplicationEngine *engine;

    explicit MainWindow(QWidget *parent = nullptr);
    void SetupUI();

signals:

public slots:    
    void SetProperty(const char* property, const QVariant value);
    void OnApplicationStateCanged(Qt::ApplicationState state);

private:
    #ifdef Q_OS_ANDROID
    /* Prevent screen from turning off - display is on as long as ther is an object of this created*/
    KeepAwakeHelper *awake = nullptr;
    #endif

    QObject *ui;
};

#endif // MAINWINDOW_H
