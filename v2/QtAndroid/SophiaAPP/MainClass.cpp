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
    this->ui->engine->rootContext()->setContextProperty("cpp", this);
}

void MainClass::SetStatus(QString description, UiStatusType status)
{
    QString statusStr = (status==UiStatusType::SUCCESS?"SUCCESS":(status==UiStatusType::ERROR?"ERROR":"PENDING"));
    qDebug() << "[" + statusStr + "] " + description;
    ui->SetProperty("statusText", statusStr + ": " + description);
}

bool MainClass::SendPacket(Packet *packet)
{
    /* reset timer */
    this->timeCounter.restart();

    if(this->rpi != nullptr && this->rpi->is_alive())
    {
        char serialized[PACKET_SIZE];
        QByteArray bytes;
        Serialize(packet, serialized);
        bytes = QByteArray(reinterpret_cast<char*>(serialized), PACKET_SIZE);

        emit _WritePacket(bytes);
    }
    return true;
}

bool MainClass::SendAck()
{
    static Packet packetAck =
    {.param = PacketParams::ACK, .value = 0x00};
    SendPacket(&packetAck);
}

void MainClass::run()
{
    timeCounter.restart();

    while(true)
    {
        /* Send ACK package every 3 seconds */
        if(timeCounter.elapsed() > 2500)
            SendAck();

        /* Unexpected connection lost? */
        if(ackTimeCounter.elapsed() > 8000)
        {
            ackTimeCounter.restart();
            if(this->rpi != nullptr && this->rpi->is_alive())
            {
                emit _DoDisconnect();
                this->rpi = nullptr;
            }
        }

        /* Prevent high CPU load */
        QThread::msleep(1);
    }
}

void MainClass::onTcpReadyRead()
{
    char recvBuffer[512];
    qint64 readBytes = rpi->read(recvBuffer, 512);

    for(int i = 0; i < readBytes/PACKET_SIZE; i++)
    {
        Packet recvPacket;
        static char tmp[PACKET_SIZE];
        memcpy(&tmp, recvBuffer + (i*PACKET_SIZE), PACKET_SIZE);
        Deserialize(tmp, PACKET_SIZE, &recvPacket);

        /* Notify the reception of new packet */
        onTcpPacketReceived(&recvPacket);
    }
}

void MainClass::onTcpConnectionChanged(bool connected)
{
    if(connected)
    {
        ackTimeCounter.restart();
        ui->SetProperty("connection_status_label", "connected");
        this->SetStatus("Connected to " + QString(RPI_IP_ADDRESS) + ":" + QString::number(RPI_PORT), UiStatusType::SUCCESS);
    }
    else
    {
        ackTimeCounter.restart();
        ui->SetProperty("connection_status_label", "NOT connected");
        this->SetStatus("Disconnected from RPi server", UiStatusType::ERROR);
    }
}

void MainClass::onTcpPacketReceived(Packet *recvPacket)
{
    /* ACK packets are just ignored for now */
    ackTimeCounter.restart();
    if(recvPacket->param == PacketParams::ACK)
        return;

    switch (recvPacket->param)
    {
        case PacketParams::SENSOR_CUTTER_LASER:
        {
            qDebug() << "Sensor LASER: " << ((recvPacket->value<1)?"false":"true");
            ui->SetProperty("laser_sensor", (recvPacket->value<1)?"false":"true" );
        }break;

        case PacketParams::SENSOR_INIT_HORIZONTAL_LEFT:
        {
            qDebug() << "Sensor LEFT: " << ((recvPacket->value<1)?"false":"true");
            ui->SetProperty("left_sensor", (recvPacket->value<1)?"false":"true" );
        }break;

        case PacketParams::SENSOR_INIT_HORIZONTAL_RIGHT:
        {
            qDebug() << "Sensor RIGHT: " << ((recvPacket->value<1)?"false":"true");
            ui->SetProperty("right_sensor", (recvPacket->value<1)?"false":"true" );
        }break;

        case PacketParams::SENSOR_INIT_VERTICAL_MASTER:
        {
            qDebug() << "Sensor MASTER: " << ((recvPacket->value<1)?"false":"true");
            ui->SetProperty("master_sensor", (recvPacket->value<1)?"false":"true" );
        }break;

        case PacketParams::SENSOR_INIT_VERTICAL_SLAVE:
        {
            qDebug() << "Sensor SLAVE: " << ((recvPacket->value<1)?"false":"true");
            ui->SetProperty("slave_sensor", (recvPacket->value<1)?"false":"true" );
        }break;

        default:
        {
            qDebug() << "Unknown packet received: [" << recvPacket->param << " | " << recvPacket->value << "]";
        }
    }
}

//     _   _ ___    ____  _                   _
//    | | | |_ _|  / ___|(_) __ _ _ __   __ _| |___
//    | | | || |   \___ \| |/ _` | '_ \ / _` | / __|
//    | |_| || |    ___) | | (_| | | | | (_| | \__ \
//     \___/|___|  |____/|_|\__, |_| |_|\__,_|_|___/
//                         |___/
void MainClass::onButtonPressed_Connect()
{
    this->SetStatus("Connecting to " + QString(RPI_IP_ADDRESS) + ":" + QString::number(RPI_PORT), UiStatusType::PENDING);
    if(this->rpi != nullptr && this->rpi->is_alive())
    {
//        this->SetStatus("Already connected!", UiStatusType::ERROR);
//        return;
        this->rpi->doDisconnect();
    }

    this->rpi = nullptr;
    this->rpi = new TcpClient();
    this->rpi->setHostname(RPI_IP_ADDRESS);
    this->rpi->setPort(RPI_PORT);

    connect(this, SIGNAL(_WritePacket(QByteArray)), rpi, SLOT(write(QByteArray)));
    connect(this, SIGNAL(_DoDisconnect()), rpi, SLOT(doDisconnect()));
    connect(rpi, SIGNAL(onConnectionChanged(bool)), this, SLOT(onTcpConnectionChanged(bool)), Qt::QueuedConnection );
    connect(rpi, SIGNAL(onReadyRead()), this, SLOT(onTcpReadyRead()), Qt::QueuedConnection );

    this->rpi->doConnect();

    if(!this->rpi->is_alive())
    {
        ui->SetProperty("statusText", "NOT connected");
        this->SetStatus(this->rpi->getLastError(), UiStatusType::ERROR);
        this->rpi = nullptr;
    }
}

void MainClass::onSwitchChanged_Valves(bool checked)
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "Valves: " << (checked?"ON":"OFF");
    static Packet packet;
    packet.param = PacketParams::ELECTROVALVES;
    packet.value = (checked?(TRUE):(FALSE));
    SendPacket(&packet);
}

void MainClass::onSwitchChanged_Cutter(bool checked)
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "Cutter: " << (checked?"ON":"OFF");
    static Packet packet;
    packet.param = PacketParams::CUTTER;
    packet.value = (checked?(TRUE):(FALSE));
    SendPacket(&packet);
}

void MainClass::onButtonPressed_Reset()
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "RESET request";
    static Packet packet;
    packet.param = PacketParams::RESET;
    SendPacket(&packet);
}

void MainClass::onButtonPressed_Lock()
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "LOCK request";
    static Packet packet;
    packet.param = PacketParams::ELECTROVALVES;
    packet.value = TRUE;
    SendPacket(&packet);
}

void MainClass::onButtonPressed_Unlock()
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "UNLOCK request";
    static Packet packet;
    packet.param = PacketParams::ELECTROVALVES;
    packet.value = FALSE;
    SendPacket(&packet);
}

void MainClass::onButtonPressed_Cut()
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "CUT request";
    static Packet packet;
    packet.param = PacketParams::CUT;
    packet.value = 1;
    SendPacket(&packet);
}

void MainClass::onButtonPressed_MoveTo(int size)
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    uint8_t bytes[2];
    bytes[0] = size & 0xFF;
    bytes[1] = (size >> 8) & 0xFF;

    qDebug() << "MOVETO " << size << " request";
    static Packet packet;
    packet.param = PacketParams::MOVETO;
    packet.value = bytes[1];
    packet.value2 = bytes[0];
    SendPacket(&packet);
}

void MainClass::onButtonPressed_Stop()
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "STOP request";
    static Packet packet;
    packet.param = PacketParams::EMERGENCY_STOP;
    packet.value = TRUE;
    SendPacket(&packet);
}

void MainClass::onButtonPressed_Up()
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "UP pressed";
    static Packet packet;
    packet.param = PacketParams::UP;
    packet.value = TRUE;
    SendPacket(&packet);
}

void MainClass::onButtonPressed_Down()
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "DOWN pressed";
    static Packet packet;
    packet.param = PacketParams::DOWN;
    packet.value = TRUE;
    SendPacket(&packet);
}

void MainClass::onButtonPressed_Right()
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "RIGHT pressed";
    static Packet packet;
    packet.param = PacketParams::RIGHT;
    packet.value = TRUE;
    SendPacket(&packet);
}

void MainClass::onButtonPressed_Left()
{
    if(!rpi || !rpi->is_alive())
        ui->SetProperty("statusText", "NOT Connected to machinery");

    qDebug() << "LEFT pressed";
    static Packet packet;
    packet.param = PacketParams::LEFT;
    packet.value = TRUE;
    SendPacket(&packet);
}

void MainClass::onButtonReleased_Up()
{

    qDebug() << "UP released";
    static Packet packet;
    packet.param = PacketParams::UP;
    packet.value = FALSE;
    SendPacket(&packet);
}

void MainClass::onButtonReleased_Down()
{
    qDebug() << "UP released";
    static Packet packet;
    packet.param = PacketParams::DOWN;
    packet.value = FALSE;
    SendPacket(&packet);
}

void MainClass::onButtonReleased_Right()
{
    qDebug() << "UP released";
    static Packet packet;
    packet.param = PacketParams::RIGHT;
    packet.value = FALSE;
    SendPacket(&packet);
}

void MainClass::onButtonReleased_Left()
{
    qDebug() << "UP released";
    static Packet packet;
    packet.param = PacketParams::LEFT;
    packet.value = FALSE;
    SendPacket(&packet);
}
