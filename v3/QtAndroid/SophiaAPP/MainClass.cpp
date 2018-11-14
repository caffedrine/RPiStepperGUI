#include "MainClass.h"

MainClass::MainClass(MainWindow *_ui)
{
    this->ui = _ui;
}

MainClass::~MainClass()
{

}


void MainClass::SetStatus(QString text, StatusType status)
{

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


