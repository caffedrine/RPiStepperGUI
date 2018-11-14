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

void MainWindow::SetProperty(const char* property, const QVariant value)
{
    this->ui->setProperty(property, value);
}

void MainWindow::onButtonPressed_Connect(QString ip, int port)
{
    qDebug() << "Connecting to " << ip << ":" << port;
}

void MainWindow::onSwitchChanged_Valves(bool checked)
{
    qDebug() << "Valves: " << (checked?"ON":"OFF");
}

void MainWindow::onSwitchChanged_Cutter(bool checked)
{
    qDebug() << "Cutter: " << (checked?"ON":"OFF");
}

void MainWindow::onButtonPressed_Reset()
{
    qDebug() << "RESET request";
}

void MainWindow::onButtonPressed_Lock()
{
    qDebug() << "LOCK request";
}

void MainWindow::onButtonPressed_Unlock()
{
    qDebug() << "UNLOCK request";
}

void MainWindow::onButtonPressed_Cut()
{
    qDebug() << "CUT request";
}

void MainWindow::onButtonPressed_MoveTo(int size)
{
    qDebug() << "MOVETO " << size << " request";
}
