//
// Created by curiosul on 10/9/18.
//

#ifndef DRIVERSCONTAINER_SERVERTCP_H
#define DRIVERSCONTAINER_SERVERTCP_H

#include "Globals.h"
#include "drivers/TcpServerAsync.h"
#include "utils/time_utils.h"
#include "peripherals/LedConnection.h"
#include "peripherals/LedTraffic.h"
#include "../../../Shared/packet.h"

class ServerTCP : public TcpServerAsync
{
public:
	client_t *client = NULL;
	
	ServerTCP() : TcpServerAsync(1337, 1)
	{
		console->info("Started TCP Server on port {0}", 1337);
		g_LedConnection.On();
	}
	
	int Send(const char *data, int len)
	{
		if( client != NULL && client->IsConnected())
		{
			g_LedTraffic.On();
			Write(client, data, len);
			g_LedTraffic.Off();
		}
	}
	
	bool SendPacket(const Packet *packet)
	{
		char data[PACKET_SIZE];
		int len = Serialize(packet, data);
		Send(data, len);
	}
	
	bool SendAck()
	{
		static Packet packet = { .param = PacketParams::ACK };
		this->SendPacket( &packet );
	}
	
private:
	void ClientConnected(TcpServerAsync::client_t *_client) override
	{
		g_TcpRecvLastMillis = TimeUtils::millis();
		client = _client;
		console->info("[{0} {1}:{2}] CONNECTED", _client->Fd, _client->Ip, _client->Port);
	}
	
	void ClientDisconnected(const TcpServerAsync::client_t *_client) override
	{
		client = nullptr;
		console->info("[{0} {1}:{2}] DISCONNECTED", _client->Fd, _client->Ip, _client->Port);
	}
	
	void DataReceived(const TcpServerAsync::client_t *client, char *data, int len) override
	{
		g_LedTraffic.On();
		/* is there a packet */
		if(len < PACKET_SIZE)
			return;
		
		/* Update variable to know exactly when was last packet received from client */
		g_TcpRecvLastMillis = TimeUtils::millis();
		//console->info("[{0} {1}:{2}] RECV({3} bytes): {4}", client->Fd, client->Ip, client->Port, len, data);
		
		/* If multiple packets were received */
		for(int i = 0; i < len/PACKET_SIZE; i++)
		{
			static Packet recvPacket;
			static char tmp[PACKET_SIZE];
			memcpy(&tmp, data + (i*PACKET_SIZE), PACKET_SIZE);
			
			if( Deserialize(tmp, PACKET_SIZE, &recvPacket) >= 0 )
			{
				/* Push a message is the packet is valid and is not ACK */
				if( recvPacket.param != PacketParams::ACK )
					this->OnPacketReceived(&recvPacket);
			}
		}
		g_LedTraffic.Off();
	}
	
	void DataSend(const TcpServerAsync::client_t *client, const char *data, int bytesSend) override
	{
		console->info("[{0} {1}:{2}] SEND({3} bytes): {4}", client->Fd, client->Ip, client->Port, bytesSend, data);
	}
	
	virtual void OnPacketReceived(Packet *packet)
	{
		/* Send back an acknowledgement */
		SendAck();
		console->info("New packet received: [{0} {1}]", packet->param, packet->value);
	}
};

ServerTCP g_TcpServer;

#endif //DRIVERSCONTAINER_SERVERTCP_H
