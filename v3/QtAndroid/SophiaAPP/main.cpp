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
    qmlRegisterType<MainClass>("QmlMainClass",1,0,"QmlMainClass");

    /* Load GUI */
    MainWindow ui;
    ui.SetupUI();

    /* Launch work on a separate thread to keep UI thread free */
    QThread main_work_thread;
    MainClass MainWork;
    QObject::connect(&MainWork, SIGNAL(UiSetProperty(const char *, QVariant)), &ui, SLOT(SetProperty(const char *, QVariant)));
    MainWork.moveToThread(&main_work_thread);
    MainWork.SetUi(&ui);
    QObject::connect(&main_work_thread, SIGNAL(started()), &MainWork, SLOT(MainLoop()) );
    main_work_thread.start();

    return app.exec();
}
