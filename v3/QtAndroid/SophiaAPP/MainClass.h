#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include <QDebug>
#include <QThread>

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
    MainClass();
    ~MainClass();

    void SetStatus(QString text, StatusType status);

private:

public slots:
    void MainLoop();
    Q_INVOKABLE void onButtonPressed_Connect(QString ip, int port);
    Q_INVOKABLE void onSwitchChanged_Valves(bool checked);
    Q_INVOKABLE void onSwitchChanged_Cutter(bool checked);
    Q_INVOKABLE void onButtonPressed_Reset();
    Q_INVOKABLE void onButtonPressed_Lock();
    Q_INVOKABLE void onButtonPressed_Unlock();
    Q_INVOKABLE void onButtonPressed_Cut();
    Q_INVOKABLE void onButtonPressed_MoveTo(int size);
};

#endif // MAINCLASS_H
