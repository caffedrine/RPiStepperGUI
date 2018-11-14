#include "MainClass.h"

MainClass::MainClass()
{
}

MainClass::~MainClass()
{

}

void MainClass::SetQmlEngine(QQmlApplicationEngine *engine)
{
    this->ui = engine->rootObjects()[0];

}

void MainClass::SetStatus(QString text, StatusType status)
{

}

void MainClass::MainLoop()
{
    while(true)
    {
        //qDebug() << "Hello!";
        QThread::msleep(500);
    }
}

void MainClass::onButtonPressed_Connect(QString ip, int port)
{
    qDebug() << "Connecting to " << ip << ":" << port;

    this->ui->setProperty("statusText", "Hello!");
}

void MainClass::onSwitchChanged_Valves(bool checked)
{
    qDebug() << "Valves: " << (checked?"ON":"OFF");
}

void MainClass::onSwitchChanged_Cutter(bool checked)
{
    qDebug() << "Cutter: " << (checked?"ON":"OFF");
}

void MainClass::onButtonPressed_Reset()
{
    qDebug() << "RESET request";
}

void MainClass::onButtonPressed_Lock()
{
    qDebug() << "LOCK request";
}

void MainClass::onButtonPressed_Unlock()
{
    qDebug() << "UNLOCK request";
}

void MainClass::onButtonPressed_Cut()
{
    qDebug() << "CUT request";
}

void MainClass::onButtonPressed_MoveTo(int size)
{
    qDebug() << "MOVETO " << size << " request";
}
