#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include <QDebug>
#include <QThread>
#include <QObject>

#include "MainClass.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    //call before any qml may use the custom component
    qmlRegisterType<MainClass>("QmlInterface",1,0,"QmlInterface");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;



    /* Launch work on a separate thread to keep UI thread free */
    QThread thread;
    MainClass work;
    work.moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(started()), &work, SLOT(MainLoop()) );

    thread.start();

    return app.exec();
}
