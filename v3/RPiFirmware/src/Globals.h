#ifndef DRIVERSCONTAINER_COMMON_H
#define DRIVERSCONTAINER_COMMON_H

#include "Config.h"
#include "../../Shared/packet.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/* Logger instance - multi-threaded */
auto console = spdlog::stdout_color_mt("console");

/* Global variable to store latest received ACK from client */
uint64_t g_TcpRecvLastMillis = 0;

/* Program states */
class States
{
public:
	enum
	{
		STANDBY,
		WAIT_RESET,
		WAIT_MOVETO,
		WAIT_CUT,
		WAIT_CUTTER_INIT,
		WAIT_LOCK,
		WAIT_UNLOCK,
		EMERGENCY_STOP,
		
		STATES_NO
	};
	
	typedef struct State
	{
		int Val;
		const char *description;
	}state_t;
	
	/* Store current and previous state */
	state_t Current, Previous;
	/* Description of each state */
	const char *states_description[STATES_NO] =
			{
					states_description[States::STANDBY] = "[STANDBY] Waiting for command",
					states_description[States::WAIT_RESET] = "[WAIT_RESET] Waiting for motors reinitialization",
					states_description[States::WAIT_MOVETO] = "[WAIT_MOVETO] Moving to target position",
					states_description[States::WAIT_CUT] = "[WAIT_CUT] Waiting to cut the curtain",
					states_description[States::WAIT_CUTTER_INIT] = "[WAIT_CUTTER_INIT] Waiting for cutter to come back",
					states_description[States::WAIT_LOCK] = "[WAIT_LOCK] Waiting electrovalves to lock curtain",
					states_description[States::WAIT_UNLOCK] = "[WAIT_UNLOCK] Wait for electrovalves to unlock curtain",
					states_description[States::EMERGENCY_STOP] = "[EMERGENCY_STOP] Emergency stop triggered",
			};
	
	States()
	{
		Current.Val = 0;
		Previous.Val = 0;
		Set(States::STANDBY);
	}
	
	void Set(int new_state)
	{
		Previous.Val = Current.Val;
		Previous.description = states_description[Current.Val];
		
		Current.Val = new_state;
		Current.description = states_description[Current.Val];
		
		/* Print current state */
		console->info( states_description[Current.Val] );
	}

private:
};

States g_State;

#endif //DRIVERSCONTAINER_COMMON_H
