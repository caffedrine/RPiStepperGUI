#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include <QDebug>
#include <QThread>
#include <QObject>

#include "MainClass.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    //call before any qml may use the custom component
    qmlRegisterType<MainWindow>("QmlInterface",1,0,"QmlInterface");

    /* Load GUI */
    MainWindow ui;
    ui.SetupUI();

    /* Launch work on a separate thread to keep UI thread free */
    QThread main_work_thread;
    MainClass MainWork(&ui);
    MainWork.moveToThread(&main_work_thread);
    QObject::connect(&main_work_thread, SIGNAL(started()), &MainWork, SLOT(MainLoop()) );
    main_work_thread.start();

    return app.exec();
}
