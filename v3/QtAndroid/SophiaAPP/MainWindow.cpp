#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
{

}

void MainWindow::SetupUI()
{
    qDebug() << "Setting up UI...";
    this->engine = new QQmlApplicationEngine();
    engine->load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine->rootObjects().isEmpty())
    {
        qDebug() << "Failed to load qml ui!";
        QCoreApplication::quit();
    }
    this->ui = engine->rootObjects()[0];
}

void MainWindow::SetStatus(QString text, UiStatusType status)
{

}

void MainWindow::SetProperty(const char* property, const QVariant value)
{
    this->ui->setProperty(property, value);
}
