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

    QThread *thread = new QThread();
    MainClass *work = new MainClass();
    work->moveToThread(thread);
    connect(thread, SIGNAL(started()), work, SLOT(mainLoop()));

    thread->start();

    return app.exec();
}
