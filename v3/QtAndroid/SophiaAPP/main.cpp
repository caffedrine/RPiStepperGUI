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

    /* Load GUI */
    MainWindow ui;
    ui.SetupUI();

    /* Display IP and Port */
    ui.SetProperty("ip_address", RPI_IP_ADDRESS);
    ui.SetProperty("port", RPI_PORT);

    /* Launch work on a separate thread to keep UI thread free */
    MainClass work;
    //QObject::connect(&MainWork, SIGNAL(UiSetProperty(const char *, QVariant)), &ui, SLOT(SetProperty(const char *, QVariant)));
    QObject::connect(&ui, SIGNAL( onConnectButtonPressed() ), &work, SLOT( onButtonPressed_Connect() ));
    //MainWork->SetUi(ui);
    work.start();

    return app.exec();
}
