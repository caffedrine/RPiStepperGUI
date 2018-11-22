//
// Created by curiosul on 10/9/18.
//

#ifndef DRIVERSCONTAINER_SERVERTCP_H
#define DRIVERSCONTAINER_SERVERTCP_H

#include <peripherals/SensorHorizontalLeft.h>
#include "Globals.h"
#include "drivers/TcpServerAsync.h"
#include "utils/time_utils.h"
#include "peripherals/LedConnection.h"
#include "peripherals/LedTraffic.h"
#include "peripherals/MasterDC.h"
#include "peripherals/SlaveDC.h"
#include "peripherals/CutterDC.h"
#include "peripherals/Cutter.h"
#include "peripherals/ElectroValves.h"
#include "peripherals/SensorVerticalMaster.h"
#include "peripherals/SensorVerticalSlave.h"
#include "peripherals/SensorHorizontalLeft.h"
#include "peripherals/SensorHorizontalRight.h"
#include "peripherals/SensorLaser.h"

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
		
		/* Reset is required */
		ResetRequired = true;
		/* Send sensors states to client */
		Packet packet;
		/* Laser */
		packet.param = PacketParams::SENSOR_CUTTER_LASER;
		packet.value = (uint8_t)g_SensorLaser.CurrentState;
		SendPacket(&packet);
		/* master */
		packet.param = PacketParams::SENSOR_INIT_VERTICAL_MASTER;
		packet.value = (uint8_t)g_SensorVerticalMaster.CurrentState;
		SendPacket(&packet);
		/* slave */
		packet.param = PacketParams::SENSOR_INIT_VERTICAL_SLAVE;
		packet.value = (uint8_t)g_SensorVerticalSlave.CurrentState;
		SendPacket(&packet);
		/* right */
		packet.param = PacketParams::SENSOR_INIT_HORIZONTAL_RIGHT;
		packet.value = (uint8_t)g_SensorHorizontalRight.CurrentState;
		SendPacket(&packet);
		/* left */
		packet.param = PacketParams::SENSOR_INIT_HORIZONTAL_LEFT;
		packet.value = (uint8_t)g_SensorHorizontalLeft.CurrentState;
		SendPacket(&packet);
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
		//console->info("[{0} {1}:{2}] SEND({3} bytes): {4}", client->Fd, client->Ip, client->Port, bytesSend, data);
	}
	
	virtual void OnPacketReceived(Packet *packet)
	{
		/* Send back an acknowledgement */
		SendAck();
		HandlePacketRecv(packet);
	}

protected:
	void HandlePacketRecv(Packet *packet)
	{
		
		/* Handle emergency stop first */
		if(packet->param == PacketParams::EMERGENCY_STOP)
		{
			console->warn("Emergency stop request received!");
			g_State.Set(States::EMERGENCY_STOP);
			return;
		}
		
		/* Only if application is not already running */
		if(g_State.Current.Val != States::STANDBY)
		{
			console->warn("Wait for current action to finish or press STOP");
			return;
		}
		
		switch(packet->param)
		{
			case PacketParams::CUTTER:
			{
				if(packet->value == (uint8_t)LogicalLevel::HIGH)
				{
					console->info("[MANUAL] Cutter ON");
					g_Cutter.On();
				}
				else
				{
					console->info("[MANUAL] Cutter OFF");
					g_Cutter.Off();
				}
			}break;
			
			case PacketParams::ELECTROVALVES:
			{
				if(packet->value == (uint8_t)LogicalLevel::HIGH)
				{
					console->info("[MANUAL] Electrovalves ON");
					g_ElectroValves.On();
				}
				else
				{
					console->info("[MANUAL] Electrovalves OFF");
					g_ElectroValves.Off();
				}
			}break;
			
			case PacketParams::UP:
			{
				g_MasterDC.SetDirection(MotorDcDirection::FORWARD);
				if(packet->value == (uint8_t)LogicalLevel::HIGH)
				{
					console->info("[MANUAL] Running UP");
					g_MasterDC.Run();
				}
				else
				{
					console->info("[MANUAL] Stopped");
					g_MasterDC.Stop();
				}
			}break;
			
			case PacketParams::DOWN:
			{
				g_MasterDC.SetDirection(MotorDcDirection::BACKWARD);
				if(packet->value == (uint8_t)LogicalLevel::HIGH)
				{
					console->info("[MANUAL] Running down");
					g_MasterDC.Run();
				}
				else
				{
					console->info("[MANUAL] Stopped");
					g_MasterDC.Stop();
				}
			}break;
			
			case PacketParams::RIGHT:
			{
				g_CutterDC.SetDirection(MotorDcDirection::FORWARD);
				if(packet->value == (uint8_t)LogicalLevel::HIGH)
				{
					console->info("[MANUAL] Running right");
					g_CutterDC.Run();
				}
				else
				{
					console->info("[MANUAL] Stopped");
					g_CutterDC.Stop();
				}
			}break;
			
			case PacketParams::LEFT:
			{
				g_CutterDC.SetDirection(MotorDcDirection::BACKWARD);
				if(packet->value == (uint8_t)LogicalLevel::HIGH)
				{
					console->info("[MANUAL] Running left");
					g_CutterDC.Run();
				}
				else
				{
					console->info("[MANUAL] Stopped");
					g_CutterDC.Stop();
				}
			}break;
			
			case PacketParams::RESET:
			{
				console->info("[MANUAL] Resetting");
				if(g_SensorVerticalMaster.CurrentState == PushButtonState::DOWN && g_SensorHorizontalLeft.CurrentState == PushButtonState::DOWN)
				{
					/* Already reseted */
					return;
				}
//
//				/* Check which one to reset */
//				if(g_SensorVertical.CurrentState == LogicalLevel::LOW)
//				{
//					g_MasterDC.SetDirection(MotorDcDirection::BACKWARD);
//					g_MasterDC.Run();
//				}
				
				
				g_State.Set(States::WAIT_RESET);
			}break;
			
			default:
			{
				console->warn("Unknown packet: [{} {}]", packet->param, packet->value);
				break;
			}
		}
	} /* Method */
}; /* Class */

ServerTCP g_TcpServer;

#endif //DRIVERSCONTAINER_SERVERTCP_H
