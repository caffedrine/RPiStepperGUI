#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
{

}

void MainWindow::SetupUI()
{
    qDebug() << "Setting up UI...";
    this->engine = new QQmlApplicationEngine();
    this->engine->load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine->rootObjects().isEmpty())
    {
        qDebug() << "Failed to load qml ui!";
        QCoreApplication::quit();
    }
    else
    {
//        engine->rootContext()->setContextProperty("cpp", this);
        this->ui = engine->rootObjects()[0];
    }
}

void MainWindow::SetProperty(const char* property, const QVariant value)
{
    this->ui->setProperty(property, value);
}

void MainWindow::OnApplicationStateCanged(Qt::ApplicationState state)
{
    qDebug() << "Application went into a new state: " << state;

    #ifdef Q_OS_ANDROID
    if(state == Qt::ApplicationState::ApplicationActive)
    {
        /* Prevent screen from turning off */
        this->awake = new KeepAwakeHelper();

    }
    else
    {
        /* Prevent screen from turning off - display is on as long as ther is an object of this created*/
        if(this->awake != nullptr)
        {
            delete awake;
            awake = nullptr;
        }
    }
    #endif
}

