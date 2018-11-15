#include "MainClass.h"

MainClass::MainClass()
{
}

MainClass::~MainClass()
{

}

void MainClass::SetUi(MainWindow *_ui)
{
    this->ui = _ui;
}

void MainClass::SetStatus(QString description, UiStatusType status)
{
    QString statusStr = (status==UiStatusType::SUCCESS?"SUCCESS":(status==UiStatusType::ERROR?"ERROR":"PENDING"));
    qDebug() << "[" + statusStr + "] " + description;
}

void MainClass::MainLoop()
{
    /* Prevent screen from turning off */
    KeepAwakeHelper helper;

    while(true)
    {
        static int i = 0;
        QString tmp = QString::number(i++) + ". Ciao!";

        //qDebug() << tmp;
        //ui->SetProperty("statusText", tmp);

        QThread::msleep(1000);
    }
}

void MainClass::onTcpReadyRead()
{

}


void MainClass::onTcpConnectionChanged(bool connected)
{
    qDebug() << "Connected slot triggered!";

    try
    {
        ui->SetProperty("statusText", "connected");
    }
    catch (std::exception &e)
    {
        qDebug() << e.what();
    }
}

void MainClass::onTcpPacketReceived(packet_t packet)
{

}

//     _   _ ___    ____  _                   _
//    | | | |_ _|  / ___|(_) __ _ _ __   __ _| |___
//    | | | || |   \___ \| |/ _` | '_ \ / _` | / __|
//    | |_| || |    ___) | | (_| | | | | (_| | \__ \
//     \___/|___|  |____/|_|\__, |_| |_|\__,_|_|___/
//                         |___/
void MainClass::onButtonPressed_Connect(QString ip, int port)
{
    this->SetStatus("Connecting to " + ip + ":" + QString::number(port), UiStatusType::PENDING);
    if(this->rpi != nullptr && this->rpi->is_alive())
    {
        this->SetStatus("Already connected!", UiStatusType::ERROR);
        return;
    }

    rpi = nullptr;
    rpi = new TcpClient();
    rpi->setHostname(ip);
    rpi->setPort(port);
    connect(rpi, SIGNAL(onConnectionChanged(bool)), this, SLOT(onTcpConnectionChanged(bool)) );
    connect(rpi, SIGNAL(onReadyRead()), this, SLOT(onTcpReadyRead()) );

    rpi->doConnect();

    if(!rpi->is_alive())
    {
        QString tmp = "connected";
        emit UiSetProperty("statusText", tmp);

        this->SetStatus(rpi->getLastError(), UiStatusType::ERROR);
        this->rpi = nullptr;
    }
    else
    {
        this->SetStatus("Connected to " + ip + ":" + QString::number(port), UiStatusType::SUCCESS);
    }
}

void MainClass::onSwitchChanged_Valves(bool checked)
{
    qDebug() << "Valves: " << (checked?"ON":"OFF");
}

void MainClass::onSwitchChanged_Cutter(bool checked)
{
    qDebug() << "Cutter: " << (checked?"ON":"OFF");
}

void MainClass::onButtonPressed_Reset()
{
    qDebug() << "RESET request";
}

void MainClass::onButtonPressed_Lock()
{
    qDebug() << "LOCK request";
}

void MainClass::onButtonPressed_Unlock()
{
    qDebug() << "UNLOCK request";
}

void MainClass::onButtonPressed_Cut()
{
    qDebug() << "CUT request";
}

void MainClass::onButtonPressed_MoveTo(int size)
{
    qDebug() << "MOVETO " << size << " request";
}

void MainClass::onButtonPressed_Up()
{
    qDebug() << "UP pressed";
}

void MainClass::onButtonPressed_Down()
{
    qDebug() << "DOWN pressed";
}

void MainClass::onButtonPressed_Right()
{
    qDebug() << "RIGHT pressed";
}

void MainClass::onButtonPressed_Left()
{
    qDebug() << "LEFT pressed";
}

void MainClass::onButtonReleased_Up()
{
    qDebug() << "UP released";
}

void MainClass::onButtonReleased_Down()
{
    qDebug() << "DOWN released";
}

void MainClass::onButtonReleased_Right()
{
    qDebug() << "RIGHT released";
}

void MainClass::onButtonReleased_Left()
{
    qDebug() << "LEFT released";
}
