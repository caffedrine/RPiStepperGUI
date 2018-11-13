#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QThread>
#include <QObject>

#include "MainClass.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    /* Launch work on a separate thread to keep UI thread free */
    QThread *thread = new QThread();
    MainClass *work = new MainClass();
    work->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), work, SLOT(MainLoop()) );
    QObject::connect( &app, SIGNAL(onButtonPress_Connect(QString buttonId)), work, SLOT(buttonPressed(QString buttonId)) );

    thread->start();

    return app.exec();
}
