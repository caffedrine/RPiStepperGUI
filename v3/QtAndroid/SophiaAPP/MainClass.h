#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QCoreApplication>


#include <QObject>
#include <QDebug>
#include <QThread>

#include "LockHelper.h"
#include "MainWindow.h"
#include "TcpClient.h"
#include "../../Shared/packet.h"

enum class UiStatusType
{
    SUCCESS = 0,
    ERROR = 1,
    PENDING = 2
};

class MainClass: public QObject
{
    Q_OBJECT
public:
    MainClass();
    ~MainClass();
    void SetUi(MainWindow *_ui);
    void SetStatus(QString description, UiStatusType);

private:
    MainWindow *ui;
    TcpClient *rpi;

signals:
    void UiSetProperty(const char* property, QVariant value);

public slots:
    void MainLoop();
    void onTcpReadyRead();
    void onTcpPacketReceived(packet_t packet);
    void onTcpConnectionChanged(bool connected);

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
    Q_INVOKABLE void onButtonPressed_Left();
    Q_INVOKABLE void onButtonReleased_Up();
    Q_INVOKABLE void onButtonReleased_Down();
    Q_INVOKABLE void onButtonReleased_Right();
    Q_INVOKABLE void onButtonReleased_Left();
};

#endif // MAINCLASS_H
