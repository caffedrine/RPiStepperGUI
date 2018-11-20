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

bool MainClass::SendPacket(bool isAck)
{
    /* reset timer */
    this->timeCounter.restart();

    if(this->rpi != nullptr && this->rpi->is_alive())
    {
        QByteArray bytes;
        /* Param byte */
        //bytes.push_back( isAck?((uint8_t)PacketParams::ACK):((uint8_t)packet.param) );
        bytes.push_back('a');
        /* Value byte  - ignored for ack packet*/
        //bytes.push_back( (uint8_t)packet.value );

        if(rpi->write(bytes) < bytes.length())
            qDebug() << "Failed to send data to rpi: " << rpi->getLastError();
        else
            qDebug() << "SEND " << bytes.length();
    }
    return false;
}

void MainClass::MainLoop()
{
    #ifdef Q_OS_ANDROID
    /* Prevent screen from turning off */
    KeepAwakeHelper helper;
    #endif

    timeCounter.restart();

    while(true)
    {
        /* Send ACK package every 3 seconds */
        if(timeCounter.elapsed() > 2000)
            SendPacket(true);

        /* Update GUI */
        //qApp->processEvents();

        /* Prevent high CPU load */
        QThread::msleep(1);
    }
}

void MainClass::run()
{
    #ifdef Q_OS_ANDROID
    /* Prevent screen from turning off */
    KeepAwakeHelper helper;
    #endif

    timeCounter.restart();

    while(true)
    {
        /* Send ACK package every 3 seconds */
        if(timeCounter.elapsed() > 2000)
            SendPacket(true);

        /* Update GUI */
        //qApp->processEvents();

        /* Prevent high CPU load */
        QThread::msleep(1);
    }
}

void MainClass::onTcpReadyRead()
{
}


void MainClass::onTcpConnectionChanged(bool connected)
{
    if(!connected)
        this->SetStatus("Disconnected from RPi server", UiStatusType::ERROR);

//    try
//    {
//        ui->SetProperty("statusText", "connected");
//    }
//    catch (std::exception &e)
//    {
//        qDebug() << e.what();
//    }
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
void MainClass::onButtonPressed_Connect()
{

    qDebug() << "Slot triggered";

    this->SetStatus("Connecting to " + QString(RPI_IP_ADDRESS) + ":" + QString::number(RPI_PORT), UiStatusType::PENDING);
    if(this->rpi != nullptr && this->rpi->is_alive())
    {
        this->SetStatus("Already connected!", UiStatusType::ERROR);
        return;
    }

    this->rpi = nullptr;
    this->rpi = new TcpClient();
    this->rpi->setHostname(RPI_IP_ADDRESS);
    this->rpi->setPort(RPI_PORT);
    connect(rpi, SIGNAL(onConnectionChanged(bool)), this, SLOT(onTcpConnectionChanged(bool)) );
    connect(rpi, SIGNAL(onReadyRead()), this, SLOT(onTcpReadyRead()) );

    this->rpi->doConnect();

    if(!this->rpi->is_alive())
    {
        emit UiSetProperty("statusText", "NOT connected");

        this->SetStatus(this->rpi->getLastError(), UiStatusType::ERROR);
        this->rpi = nullptr;
    }
    else
    {
        this->SetStatus("Connected to " + QString(RPI_IP_ADDRESS) + ":" + QString::number(RPI_PORT), UiStatusType::SUCCESS);
    }
}

void MainClass::onSwitchChanged_Valves(bool checked)
{
    qDebug() << "Valves: " << (checked?"ON":"OFF");
    packet.param = PacketParams::ELECTROVALVES;
    packet.value = (checked?(TRUE):(FALSE));
    SendPacket();
}

void MainClass::onSwitchChanged_Cutter(bool checked)
{
    qDebug() << "Cutter: " << (checked?"ON":"OFF");
    packet.param = PacketParams::CUTTER;
    packet.value = (checked?(TRUE):(FALSE));
    SendPacket();
}

void MainClass::onButtonPressed_Reset()
{
    qDebug() << "RESET request";
    packet.param = PacketParams::RESET;
    SendPacket();
}

void MainClass::onButtonPressed_Lock()
{
    qDebug() << "LOCK request";
    packet.param = PacketParams::ELECTROVALVES;
    packet.value = TRUE;
    SendPacket();
}

void MainClass::onButtonPressed_Unlock()
{
    qDebug() << "UNLOCK request";
    packet.param = PacketParams::ELECTROVALVES;
    packet.value = FALSE;
    SendPacket();
}

void MainClass::onButtonPressed_Cut()
{
    qDebug() << "CUT request";
    packet.param = PacketParams::CUT;
    SendPacket();
}

void MainClass::onButtonPressed_MoveTo(int size)
{
    qDebug() << "MOVETO " << size << " request";
    packet.param = PacketParams::MOVETO;
    packet.value = TRUE;
    SendPacket();
}

void MainClass::onButtonPressed_Up()
{
    qDebug() << "UP pressed";
    packet.param = PacketParams::UP;
    packet.value = TRUE;
    SendPacket();
}

void MainClass::onButtonPressed_Down()
{
    qDebug() << "DOWN pressed";
    packet.param = PacketParams::DOWN;
    packet.value = TRUE;
    SendPacket();
}

void MainClass::onButtonPressed_Right()
{
    qDebug() << "RIGHT pressed";
    packet.param = PacketParams::RIGHT;
    packet.value = TRUE;
    SendPacket();
}

void MainClass::onButtonPressed_Left()
{
    qDebug() << "LEFT pressed";
    packet.param = PacketParams::LEFT;
    packet.value = TRUE;
    SendPacket();
}

void MainClass::onButtonReleased_Up()
{
    qDebug() << "UP released";
    packet.param = PacketParams::UP;
    packet.value = FALSE;
    SendPacket();
}

void MainClass::onButtonReleased_Down()
{
    qDebug() << "UP released";
    packet.param = PacketParams::DOWN;
    packet.value = FALSE;
    SendPacket();
}

void MainClass::onButtonReleased_Right()
{
    qDebug() << "UP released";
    packet.param = PacketParams::RIGHT;
    packet.value = FALSE;
    SendPacket();
}

void MainClass::onButtonReleased_Left()
{
    qDebug() << "UP released";
    packet.param = PacketParams::LEFT;
    packet.value = FALSE;
    SendPacket();
}
