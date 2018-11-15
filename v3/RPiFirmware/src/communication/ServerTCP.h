//
// Created by curiosul on 10/9/18.
//

#ifndef DRIVERSCONTAINER_SERVERTCP_H
#define DRIVERSCONTAINER_SERVERTCP_H

#include "Common.h"
#include "drivers/TcpServerAsync.h"
#include "utils/time_utils.h"

class ServerTCP : public TcpServerAsync
{
public:
	client_t *client = NULL;
	
	ServerTCP() : TcpServerAsync(1337, 1)
	{
		console->info("Started TCP Server on port {0}", 1337);
	}
	
	int Send(const char *data, int len)
	{
		if( client != NULL && client->IsConnected())
		{
			Write(client, data, len);
		}
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
		/* Update variable to know exactly when was last packet received from client */
		g_TcpRecvLastMillis = TimeUtils::millis();
		
		console->info("[{0} {1}:{2}] RECV({3} bytes): {4}", client->Fd, client->Ip, client->Port, len, data);
	}
	
	void DataSend(const TcpServerAsync::client_t *client, const char *data, int bytesSend) override
	{
		console->info("[{0} {1}:{2}] SEND({3} bytes): {4}", client->Fd, client->Ip, client->Port, bytesSend, data);
	}
};

ServerTCP g_TcpServer;

#endif //DRIVERSCONTAINER_SERVERTCP_H
