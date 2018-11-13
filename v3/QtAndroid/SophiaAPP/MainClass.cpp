#include "MainClass.h"

MainClass::MainClass()
{

}

MainClass::~MainClass()
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

void MainClass::buttonPress(QString button_id)
{
    qDebug() << "Clicked button: " << button_id;
}
