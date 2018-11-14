#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QCoreApplication>

class MainWindow : public QObject
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void SetupUI();
    void SetProperty(const char* property, const QVariant value);

signals:

public slots:    
    Q_INVOKABLE void onButtonPressed_Connect(QString ip, int port);
    Q_INVOKABLE void onSwitchChanged_Valves(bool checked);
    Q_INVOKABLE void onSwitchChanged_Cutter(bool checked);
    Q_INVOKABLE void onButtonPressed_Reset();
    Q_INVOKABLE void onButtonPressed_Lock();
    Q_INVOKABLE void onButtonPressed_Unlock();
    Q_INVOKABLE void onButtonPressed_Cut();
    Q_INVOKABLE void onButtonPressed_MoveTo(int size);

private:
    QQmlApplicationEngine *engine;
    QObject *ui;
};

#endif // MAINWINDOW_H
