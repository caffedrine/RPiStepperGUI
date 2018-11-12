#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include <QDebug>
#include <QThread>

class MainClass: public QObject
{
    Q_OBJECT
public:
    MainClass();
    ~MainClass();

    void Start()
    {
        while(true)
        {
            qDebug() << "Hello!";
            QThread::msleep(500);
        }

    }

private:

public slots:
    void onButtonPress_Connect()
    {

    }
};

#endif // MAINCLASS_H
