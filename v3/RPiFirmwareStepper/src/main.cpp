#include <iostream>
#include <signal.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

#include "utils/time_utils.h"
#include "Globals.h"
#include "cdd/Motion.h"
#include "communication/ServerTCP.h"
#include "peripherals/CutterDC.h"
#include "peripherals/Cutter.h"
#include "peripherals/ElectroValves.h"
#include "peripherals/LedConnection.h"
#include "peripherals/LedTraffic.h"
#include "peripherals/StepperMotor.h"
#include "peripherals/SensorVerticalMaster.h"
#include "peripherals/SensorVerticalSlave.h"
#include "peripherals/SensorHorizontalLeft.h"
#include "peripherals/SensorHorizontalRight.h"
#include "peripherals/SensorLaser.h"
#include "peripherals/MasterEncoder.h"
#include "peripherals/SlaveEncoder.h"

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
	g_Vertical.Stop();
	g_State.Set(States::EMERGENCY_STOP);
	g_LedConnection.Off();
	g_LedTraffic.Off();
	g_MasterDC.Stop();
	g_SlaveDC.Stop();
	g_CutterDC.Stop();
	g_Cutter.Off();
	g_ElectroValves.Off();
	
	console->info("Executing STOP routine...");
}

void SigHandler(int signum)
{
	std::cout << std::endl;
	console->critical("Interrupt signal ({0} - {1}) received", strsignal(signum), signum);
	OnExit();
	_ProgramContinue = false;
	//exit(signum);
}

void Fault( const char *msg)
{
	console->critical("[FAULT] Program entered on a faulty state");
	console->critical("[FAULT] {0}", msg);
	
	/* Stop everything */
	OnExit();
	
	while(true)
	{
		if( !_ProgramContinue )
			break;
		
		g_LedTraffic.On();
		g_LedConnection.Off();
		std::this_thread::sleep_for( std::chrono::milliseconds(200) );
		
		g_LedTraffic.Off();
		g_LedConnection.On();
		std::this_thread::sleep_for( std::chrono::milliseconds(200) );
	}
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


void Tick()
{
	g_SensorVerticalMaster.Tick();
	g_SensorVerticalSlave.Tick();
	g_SensorHorizontalLeft.Tick();
	g_SensorHorizontalRight.Tick();
	g_SensorLaser.Tick();
	g_LedTraffic.Tick();
	
	/* Tick vertical movement motors */
	g_Vertical.Tick();
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
			HandleVerticalStop();
			g_Vertical.Stop();
			g_Cutter.Off();
			g_ElectroValves.Off();
			g_CutterDC.Stop();
			g_State.Set(States::STANDBY);
		}
		
		/* Handle WAIT_RESET state */
		else if(g_State.Current.Val == States::WAIT_RESET)
		{
			/* Cutter is on initial position */
			if(g_SensorHorizontalLeft.CurrentState == PushButtonState::DOWN)
			{
				g_CutterDC.Stop();
			}
			
			/* If both are stopped then reset is finished */
			if(g_SensorHorizontalLeft.CurrentState == PushButtonState::DOWN && !g_Vertical.IsRunning())
			{
				console->info("[RESET] Success!");
				g_State.Set(States::STANDBY);
			}
			
			/* Reset can't take more than 20s */
			if( g_WaitTimer.ElapsedMs() > 25000)
			{
				console->info("Elapsed ms: {}", g_WaitTimer.ElapsedMs());
				Fault("Reset can't take more than 25 seconds");
			}
		}
		
		/* Handle WAIT_MOVETO state */
		else if(g_State.Current.Val == States::WAIT_MOVETO)
		{
			/* If required position is reached then just stop */
			if(!g_Vertical.IsRunning())
			{
				g_State.Set(States::STANDBY);
			}
			
			/* Moving to tsrget can't take more than 35s */
			if( g_WaitTimer.ElapsedMs() > 35000)
			{
				console->info("Elapsed ms: {}", g_WaitTimer.ElapsedMs());
				Fault("Too much time elapsed without reaching target position!");
			}
		}
		
		/* Handle WAIT_CUT state */
		else if(g_State.Current.Val == States::WAIT_CUT)
		{
			/* Wait 2 seconds for cutter to start */
			if( g_WaitTimer.ElapsedMs() > 2000)
			{
				g_CutterDC.SetDirection(MotorDcDirection::FORWARD);
				g_CutterDC.Run();
				
				/* End of line reached - send it back */
				if( g_SensorHorizontalRight.CurrentState == PushButtonState::DOWN )
				{
					g_CutterDC.SetDirection(MotorDcDirection::BACKWARD);
					g_State.Set(States::WAIT_CUTTER_INIT);
				}
				
				/* Wait 3 seconds before checking the laser (still need to count previous elapsed time) */
				if( g_WaitTimer.ElapsedMs() > (2000+3000) )
				{
					/* If laser does not detect courtain */
					if( !g_SensorLaser.IsCourtainPresent() )
					{
						g_CutterDC.SetDirection(MotorDcDirection::BACKWARD);
						g_Cutter.OpenCircuit();
						g_State.Set(States::WAIT_CUTTER_INIT);
					}
					
					/* Check if too much time elapsed */
					if( g_WaitTimer.ElapsedMs() > 30000)
					{
						console->info("Elapsed ms: {}", g_WaitTimer.ElapsedMs());
						Fault("Too much time took to cut curtain!");
					}
				}
			}
		}
		
		/* Handle WAIT_CUTTER_INIT state */
		else if(g_State.Current.Val == States::WAIT_CUTTER_INIT)
		{
			if(g_SensorHorizontalLeft.CurrentState == PushButtonState::DOWN)
			{
				g_CutterDC.Stop();
				g_State.Set(States::STANDBY);
			}
			
			/* If it take more than 20 seconds to reset then there it's a fault on a button or something */
			if( g_WaitTimer.ElapsedMs() > 25000)
			{
				console->info("Elapsed ms: {}", g_WaitTimer.ElapsedMs());
				Fault("Cutter failed to reach initial point in maximum allowed time!");
			}
		}
		
		/* Handle WAIT_LOCK state */
		else if(g_State.Current.Val == States::WAIT_LOCK)
		{
			if( g_WaitTimer.ElapsedMs() > 1000)
				g_State.Set(States::STANDBY);
		}
		
		/* Handle WAIT_UNLOCK state */
		else if(g_State.Current.Val == States::WAIT_UNLOCK)
		{
			if( g_WaitTimer.ElapsedMs() > 1000)
				g_State.Set(States::STANDBY);
		}
		
		/* Call modules main functions */
		Tick();
		
		/* Prevent excessive CPU load */
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}