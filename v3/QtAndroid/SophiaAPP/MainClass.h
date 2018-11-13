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

private:

public slots:
    void MainLoop();
    void buttonPress(QString button_id);
};

#endif // MAINCLASS_H
