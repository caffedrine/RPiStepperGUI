#include <iostream>
#include <signal.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

#include "utils/time_utils.h"
#include "Globals.h"
#include "communication/ServerTCP.h"
#include "peripherals/CutterDC.h"
#include "peripherals/Cutter.h"
#include "peripherals/ElectroValves.h"
#include "peripherals/LedConnection.h"
#include "peripherals/LedTraffic.h"
#include "peripherals/MasterDC.h"
#include "peripherals/SlaveDC.h"
#include "peripherals/SensorVerticalMaster.h"
#include "peripherals/SensorVerticalSlave.h"
#include "peripherals/SensorHorizontalLeft.h"
#include "peripherals/SensorHorizontalRight.h"
#include "peripherals/SensorLaser.h"
#include "peripherals/MasterEncoder.h"
#include "peripherals/SlaveEncoder.h"

bool _ProgramContinue = true;

/* Callbacks */
void SensorVerticalSlaveCallback(PushButtonState new_state)
{
	console->info("Sensor VERTICAL SLAVE state: {0}", (bool)new_state);
	static Packet packet = { .param = PacketParams::SENSOR_INIT_VERTICAL_SLAVE };
	packet.value = (uint8_t)new_state;
	g_TcpServer.SendPacket(&packet);
}

void SensorVerticalMasterCallback(PushButtonState new_state)
{
	console->info("Sensor VERTICAL MASTER state: {0}", (bool)new_state);
	static Packet packet = { .param = PacketParams::SENSOR_INIT_VERTICAL_MASTER };
	packet.value = (uint8_t)new_state;
	g_TcpServer.SendPacket(&packet);
}

void SensorHorizontalLeftCallback(PushButtonState new_state)
{
	console->info("Sensor HORIZONTAL LEFT state: {0}", (bool)new_state);
	static Packet packet = { .param = PacketParams::SENSOR_INIT_HORIZONTAL_LEFT };
	packet.value = (uint8_t)new_state;
	g_TcpServer.SendPacket(&packet);
}

void SensorHorizontalRightCallback(PushButtonState new_state)
{
	console->info("Sensor HORIZONTAL RIGHT state: {0}", (bool)new_state);
	static Packet packet = { .param = PacketParams::SENSOR_INIT_HORIZONTAL_RIGHT };
	packet.value = (uint8_t)new_state;
	g_TcpServer.SendPacket(&packet);
}

void SensorLaserCallback(LogicalLevel new_level)
{
	console->info("Sensor LASER state: {0}", (bool)new_level);
	static Packet packet = { .param = PacketParams::SENSOR_CUTTER_LASER };
	packet.value = (uint8_t)new_level;
	g_TcpServer.SendPacket(&packet);
}


void OnExit()
{
	g_LedConnection.Off();
	g_LedTraffic.Off();
	g_MasterDC.Stop();
	g_SlaveDC.Stop();
	g_CutterDC.Stop();
	g_Cutter.Off();
	
	console->info("Executing STOP routine...");
}

void SigHandler(int signum)
{
	std::cout << std::endl;
	console->critical("Interrupt signal ({0} - {1}) received", strsignal(signum), signum);
	OnExit();
	_ProgramContinue = false;
	exit(signum);
}

void Initialize()
{
	/* Handle interruption signals */
	signal(SIGINT, SigHandler);
	signal(SIGQUIT, SigHandler);
	signal(SIGTSTP, SigHandler);
	
	/* Init callbacks */
	g_SensorLaser.SetStateChangedCallback( SensorLaserCallback );
	g_SensorHorizontalLeft.SetStateChangedCallback( SensorHorizontalLeftCallback );
	g_SensorHorizontalRight.SetStateChangedCallback( SensorHorizontalRightCallback );
	g_SensorVerticalMaster.SetStateChangedCallback( SensorVerticalMasterCallback );
	g_SensorVerticalSlave.SetStateChangedCallback( SensorVerticalSlaveCallback );
}


/*
 * TODO: MUTEX MUTEX MUTEX to be added
 */
 

int main()
{
	console->info("Started main()...");
	Initialize();
	
	while( true )
	{
		if( !_ProgramContinue )
			break;
	
		/* Check for client's responsiveness */
		if(g_TcpServer.client != NULL && g_TcpServer.client->IsConnected() > 0 && TimeUtils::millis() - g_TcpRecvLastMillis > TCP_ACK_INTERVAL_MS)
		{
			console->warn("Client ACK timeout reached", TCP_ACK_INTERVAL_MS);
			console->warn("Dropped {0}:{1}", g_TcpServer.client->Ip, g_TcpServer.client->Port);
			/* Drop client connection socket */
			g_TcpServer.client->Disconnect();
			/* Go to emergency stop until next client is connected */
			g_State.Set(States::EMERGENCY_STOP);
		}
		
		/* Handle EMERGENCY_STOP state */
		if(g_State.Current.Val == States::EMERGENCY_STOP)
		{
			g_Cutter.Off();
			g_CutterDC.Stop();
			g_MasterDC.Stop();
			g_SlaveDC.Stop();
			
			g_State.Set(States::STANDBY);
		}
		
		/* Handle WAIT_RESET state */
		else if(g_State.Current.Val == States::WAIT_RESET)
		{
		
		}
		
		/* Handle WAIT_MOVETO state */
		else if(g_State.Current.Val == States::WAIT_MOVETO)
		{
		
		}
		
		/* Handle WAIT_CUT state */
		else if(g_State.Current.Val == States::WAIT_CUT)
		{
		
		}
		
		/* Handle WAIT_CUTTER_INIT state */
		else if(g_State.Current.Val == States::WAIT_CUTTER_INIT)
		{
		
		}
		
		/* Handle WAIT_LOCK state */
		else if(g_State.Current.Val == States::WAIT_LOCK)
		{
		
		}
		
		/* Handle WAIT_UNLOCK state */
		else if(g_State.Current.Val == States::WAIT_UNLOCK)
		{
		
		}
		
		/* Prevent excessive CPU load */
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}