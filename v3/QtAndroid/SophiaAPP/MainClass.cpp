#include "MainClass.h"

MainClass::MainClass()
{
    /* Connect signals */
    //connect(ui, SIGNAL(test()), this, SLOT(Test()));
}

MainClass::~MainClass()
{

}

void MainClass::SetUi(MainWindow *_ui)
{
    this->ui = _ui;
}

void MainClass::MainLoop()
{
    while(true)
    {
        static int i = 0;
        QString tmp = QString::number(i++) + ". Ciao!";

        qDebug() << tmp;
        ui->SetProperty("statusText", tmp);

        QThread::msleep(1000);
    }
}

void MainClass::onButtonPressed_Connect(QString ip, int port)
{
    qDebug() << "Connecting too " << ip << ":" << port;
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

void MainClass::onButtonPressed_Up()
{
    qDebug() << "UP pressed";
}

void MainClass::onButtonPressed_Down()
{
    qDebug() << "DOWN pressed";
}

void MainClass::onButtonPressed_Right()
{
    qDebug() << "RIGHT pressed";
}

void MainClass::onButtonPressed_Left()
{
    qDebug() << "LEFT pressed";
}

void MainClass::onButtonReleased_Up()
{
    qDebug() << "UP released";
}

void MainClass::onButtonReleased_Down()
{
    qDebug() << "DOWN released";
}

void MainClass::onButtonReleased_Right()
{
    qDebug() << "RIGHT released";
}

void MainClass::onButtonReleased_Left()
{
    qDebug() << "LEFT released";
}
