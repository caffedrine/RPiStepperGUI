//
// Created by curiosul on 11/23/18.
//

#ifndef RPIFIRMWARE_VERTICALMOVEMENT_H
#define RPIFIRMWARE_VERTICALMOVEMENT_H

#include <mutex>
#include <condition_variable>

#include "Globals.h"
#include "drivers/Pid.h"
#include "peripherals/MasterEncoder.h"
#include "peripherals/MasterEncoderAux.h"
#include "peripherals/SlaveEncoder.h"
#include "peripherals/SlaveEncoderAux.h"
#include "peripherals/MasterDC.h"
#include "peripherals/SlaveDC.h"
#include "peripherals/SensorVerticalMaster.h"
#include "peripherals/SensorVerticalSlave.h"

;
//class VerticalMovement : protected MasterEncoder, protected SlaveEncoder, protected  MasterEncoderAux, protected SlaveEncoderAux
class VerticalMovement : protected MasterEncoder, protected SlaveEncoder
{
	enum class States
	{
		STOPPED,
		RESETTING,
		RUNNING_STEPS_UP,
		RUNNING_STEPS_DOWN
	};
	
	enum class SlaveState
	{
		STOPPED,
		RUNNING
	};
	
//	 ____  _   _ ____  _     ___ ____
//	|  _ \| | | | __ )| |   |_ _/ ___|
//	| |_) | | | |  _ \| |    | | |
//	|  __/| |_| | |_) | |___ | | |___
//	|_|    \___/|____/|_____|___\____|
public:
	States State = States::STOPPED;
	
	//explicit VerticalMovement() : MasterEncoder(ENCODER_MASTER_GPIO), SlaveEncoder(ENCODER_SLAVE_GPIO), MasterEncoderAux(ENCODER_MASTER_AUX_GPIO), SlaveEncoderAux(ENCODER_SLAVE_AUX_GPIO)
	explicit VerticalMovement() : MasterEncoder(ENCODER_MASTER_GPIO), SlaveEncoder(ENCODER_SLAVE_GPIO)
	{
		BackgroundWorker = std::thread([this]()
									   {
										   BackgroundWork();
									   });
		BackgroundWorker.detach();
		
		/* Read encoders on a separate thread */
//		EncReader = std::thread([this]()
//								{
//									EncReaderThread();
//								});
//		EncReader.detach();
	
	MasterCurrentPosition = Mm2Enc(VERTICAL_MM_OFFSET);
	TargetPosition = Mm2Enc(VERTICAL_MM_OFFSET);
	SlaveCurrentPosition = Mm2Enc(VERTICAL_MM_OFFSET);

	}
	
	int32_t GetCurrentPosition()
	{
		return GetMasterCurrentPosition();
	}
	
	int32_t GetTargetPosition()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		int32_t tmp = TargetPosition;
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
		return tmp;
	}
	
	int32_t GetMasterCurrentPosition()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		int32_t tmp = MasterCurrentPosition;
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
		return tmp;
	}
	
	int32_t GetSlaveCurrentPosition()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		int32_t tmp = SlaveCurrentPosition;
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
		return tmp;
	}
	
	bool IsRunning()
	{
		if(State == States::STOPPED)
			return false;
		return true;
	}
	
	float GetCurrPositionMM()
	{
		return Enc2Mm( GetCurrentPosition() );
	}
	
	void MoveToMM(int32_t mm)
	{
		MoveTo(Mm2Enc(mm));
	}
	
	void MoveTo(int32_t encoder_units)
	{
		if( (int)( (int)(encoder_units - (int)Mm2Enc(VERTICAL_MM_OFFSET)) ) < 0)
		{
			console->warn("Can't go to a negative position: {}", ((int)( (int)(encoder_units - (int)Mm2Enc(VERTICAL_MM_OFFSET)))) );
			return;
		}
		
//		console->info("------------------------------");
		console->info("[VERTICAL] Moving from {}enc to {}enc", GetCurrentPosition(), encoder_units );
		console->info("[VERTICAL] Moving from {}mm to {}mm", Enc2Mm(GetCurrentPosition()), Enc2Mm(encoder_units) );
//		console->info("------------------------------");
		
		g_MasterDC.SetSpeed(DC_MOTOR_DEFAULT_SPEED);
		int32_t target = (encoder_units);// + Mm2Enc(VERTICAL_MM_OFFSET));
		SetTargetPosition( target );
		
		if(GetCurrentPosition() < target)
		{
			g_MasterDC.SetDirection(MotorDcDirection::FORWARD);
			g_SlaveDC.SetSpeed(DC_MOTOR_DEFAULT_SPEED);
			State = States::RUNNING_STEPS_UP;
		}
		else if(GetCurrentPosition() > target)
		{
			g_MasterDC.SetDirection(MotorDcDirection::BACKWARD);
			g_SlaveDC.SetSpeed(DC_MOTOR_DOWN_SPEED);
			State = States::RUNNING_STEPS_DOWN;
		}
	}
	
	void MoveUp()
	{
		MoveTo( Mm2Enc(VERTICAL_MAX_POSITION_MM) );
	}
	
	void MoveDown()
	{
		MoveTo( VERTICAL_MIN_POSITION );
	}
	
	void Stop()
	{
		/*  Stop */
		this->State = States::STOPPED;
		g_MasterDC.Stop();
		g_SlaveDC.Stop();
	}
	
	void Reset()
	{
		if(g_SensorVerticalMaster.CurrentState != PushButtonState::DOWN)
		{
			g_MasterDC.SetDirection(MotorDcDirection::BACKWARD);
			g_MasterDC.SetSpeed(DC_MOTOR_MASTER_RESET_SPEED);
			g_MasterDC.Run();
		}
		
		if(g_SensorVerticalSlave.CurrentState != PushButtonState::DOWN)
		{
			g_SlaveDC.SetDirection(MotorDcDirection::BACKWARD);
			g_SlaveDC.SetSpeed(DC_MOTOR_SLAVE_RESET_SPEED);
			g_SlaveDC.Run();
		}
		State = States::RESETTING;
	}
	
	static float Enc2Mm(int32_t enc)
	{
		if(enc == 0)
			return 0;
		
		//console->info("{}enc is {}mm", enc, ((float) enc * MM_PER_STEP));
		return ((float) enc * MM_PER_STEP);
	}
	
	static int32_t Mm2Enc(float mm)
	{
		if(mm == 0)
			return 0;
		
		//console->info("{}mm is {}enc", mm, ((uint32_t) (mm / MM_PER_STEP)));
		return ((int32_t) (mm / MM_PER_STEP));
	}
	
	void Tick()
	{
//		MasterEncoder::Tick();
//		SlaveEncoder::Tick();
//		MasterEncoderAux::Tick();
//		SlaveEncoderAux::Tick();
		
	}
	
//	 ____  ____  _____     ___  _____ _____
//	|  _ \|  _ \|_ _\ \   / / \|_   _| ____|
//	| |_) | |_) || | \ \ / / _ \ | | |  _|
//	|  __/|  _ < | |  \ V / ___ \| | | |___
//	|_|   |_| \_\___|  \_/_/   \_\_| |_____|
private:
	const int32_t VERTICAL_MIN_POSITION = Mm2Enc(VERTICAL_MM_OFFSET);
	
	/* PID constants */
	const double DT = 0.1;    // loop interval time
	const double MAX = 200;        // maximum value of manipulated variable
	const double MIN = 100;        // minimum value of manipulated variable
	const double KP = 0.1;        // proportional gain
	const double KD = 0.01;        // derivative gain
	const double KI = 0.4;        // Integral gain
	PID pid = PID(DT, MAX, MIN, KP, KD, KI);
	
	/* Store positions */
	int32_t MasterCurrentPosition, TargetPosition;
	/* Slave motor position */
	int32_t SlaveCurrentPosition;
	
	/* Safe read/write positions */
	std::mutex mutex_;
	std::condition_variable cond_;
	
	/* Background thread */
	std::thread BackgroundWorker;
	std::thread EncReader;
	
	void EncReaderThread()
	{
		while(true)
		{
			if( !_ProgramContinue )
				break;
			
			Tick();
			
			std::this_thread::sleep_for( std::chrono::microseconds(200)  );
		}
	}
	
	void BackgroundWork()
	{
		while( true )
		{
			if( !_ProgramContinue )
			{
				g_MasterDC.Stop();
				g_SlaveDC.Stop();
				break;
			}
			
			/* Handle reset position */
			if(State == States::RESETTING)
			{
				if(g_SensorVerticalMaster.CurrentState == PushButtonState::DOWN)
				{
					g_MasterDC.Stop();
				}
				else
				{
					g_MasterDC.SetDirection(MotorDcDirection::BACKWARD);
					g_MasterDC.SetSpeed(DC_MOTOR_MASTER_RESET_SPEED);
					g_MasterDC.Run();
				}
				
				if(g_SensorVerticalSlave.CurrentState ==  PushButtonState::DOWN)
				{
					g_SlaveDC.Stop();
				}
				else
				{
					g_SlaveDC.SetDirection(MotorDcDirection::BACKWARD);
					g_SlaveDC.SetSpeed(DC_MOTOR_SLAVE_RESET_SPEED);
					g_SlaveDC.Run();
				}
				
				if(g_SensorVerticalMaster.CurrentState == PushButtonState::DOWN && g_SensorVerticalSlave.CurrentState == PushButtonState::DOWN )
				{
					g_MasterDC.Stop();
					g_SlaveDC.Stop();
					/* Optionally reset encoders */
					MasterEncoder::Reset();
					SlaveEncoder::Reset();
					this->State = States::STOPPED;
					/* Move motors to init buttons then stop */
					SetMasterCurrentPosition( Mm2Enc(VERTICAL_MM_OFFSET) );
					SetSlaveCurrentPosition( Mm2Enc(VERTICAL_MM_OFFSET) );
					SetTargetPosition( Mm2Enc(VERTICAL_MM_OFFSET) );
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}
			
			/* Handle running steps UP */
			if(State == States::RUNNING_STEPS_UP)
			{
				if( GetMasterCurrentPosition() >= GetTargetPosition() )
				{
					/* Move down */
					g_MasterDC.Stop();
					State = States::STOPPED;
					onStepsDone();
				}
				else
				{
					/* Slow down at the last 10 steps*/
					if( (GetTargetPosition() - GetMasterCurrentPosition()) <= 20 )
					{
						g_MasterDC.SetSpeed(DC_MOTOR_DEFAULT_SPEED-20);
					}
					
					g_MasterDC.SetDirection(MotorDcDirection::FORWARD);
					g_MasterDC.Run();
				}
			}
			
			/* Handle running steps down */
			if(State == States::RUNNING_STEPS_DOWN)
			{
				if( GetMasterCurrentPosition() <= GetTargetPosition() )
				{
					/* Move down */
					g_MasterDC.Stop();
					State = States::STOPPED;
					onStepsDone();
				}
				else
				{
					/* Slow down at the last 10 steps*/
					if( ( GetMasterCurrentPosition() - GetTargetPosition() ) <= 20 )
					{
						g_MasterDC.SetSpeed(DC_MOTOR_DEFAULT_SPEED-20);
					}
					
					g_MasterDC.SetSpeed(DC_MOTOR_DOWN_SPEED);
					g_MasterDC.SetDirection(MotorDcDirection::BACKWARD);
					g_MasterDC.Run();
				}
			}
			
			/* Update encoders and others */
			//Tick();
			/* Update slave(s) is any */
			HandleSlave();
			
			std::this_thread::sleep_for(std::chrono::milliseconds( 1 ));
		}
	}
	
	void HandleSlave()
	{
		/**
		 *  Slave have the task to monitor master's position and stick to it's position
		 */
		
		/* If master is not running and slave is in correct position just ignore the rest */
		if(!IsRunning() && ( (GetSlaveCurrentPosition() >= (GetMasterCurrentPosition()-SLAVE_MOTOR_MAX_ERROR_STEP)) && (GetSlaveCurrentPosition() <= (GetMasterCurrentPosition()+SLAVE_MOTOR_MAX_ERROR_STEP))  )
		)
		{
			g_SlaveDC.Stop();
			return;
		}
		
		/* Set correct direction */
		if( GetMasterCurrentPosition() < GetSlaveCurrentPosition())
		{
			g_SlaveDC.SetDirection(MotorDcDirection::BACKWARD);
		}
		else if( GetMasterCurrentPosition() > GetSlaveCurrentPosition())
		{
			g_SlaveDC.SetDirection(MotorDcDirection::FORWARD);
		}
		
		/* Move motor to master's position */
		if((GetSlaveCurrentPosition() >= (GetMasterCurrentPosition()-SLAVE_MOTOR_MAX_ERROR_STEP)) && (GetSlaveCurrentPosition() <= (GetMasterCurrentPosition()+SLAVE_MOTOR_MAX_ERROR_STEP))  )
			//if( GetSlaveCurrentPosition() == GetMasterCurrentPosition() )
		{
			g_SlaveDC.Stop();
		}
		else
		{
			double NewSpeed = pid.calculate(GetMasterCurrentPosition(), GetSlaveCurrentPosition());
			//g_SlaveDC.SetSpeed((uint8_t) NewSpeed);
			g_SlaveDC.Run();
		}
	}
//	 ____  ____   ___ _____ _____ ____ _____ _____ ____
//	|  _ \|  _ \ / _ \_   _| ____/ ___|_   _| ____|  _ \
//  | |_) | |_) | | | || | |  _|| |     | | |  _| | | | |
//	|  __/|  _ <| |_| || | | |__| |___  | | | |___| |_| |
//	|_|   |_| \_\\___/ |_| |_____\____| |_| |_____|____/
protected:
	void OnMasterEncoderStep() override
	{
		if(g_MasterDC.CurrentDirection == MotorDcDirection::FORWARD)
		{
			this->SetMasterCurrentPosition(GetCurrentPosition() + 1);
		}
		else if(g_MasterDC.CurrentDirection == MotorDcDirection::BACKWARD)
		{
			if(GetCurrentPosition() > 0)
				this->SetMasterCurrentPosition(GetCurrentPosition() - 1);
		}
		
		if(GetMasterCurrentPosition() == GetTargetPosition())
		{
			g_MasterDC.Stop();
		}
		//console->info("Master current position: {}", GetCurrentPosition());
	}
	
//	void OnMasterEncoderAuxStep() override
//	{
//		console->info("Master AUX step!");
//	}
	
	void OnSlaveEncoderStep() override
	{
		if(g_SlaveDC.CurrentDirection == MotorDcDirection::FORWARD)
		{
			this->SetSlaveCurrentPosition( GetSlaveCurrentPosition() + 1 );
		}
		else if(g_SlaveDC.CurrentDirection == MotorDcDirection::BACKWARD)
		{
			if(this->SlaveCurrentPosition > 0)
				this->SetSlaveCurrentPosition( GetSlaveCurrentPosition() - 1 );
		}
		//console->info("Slave current position: {}", this->SlaveCurrentPosition);
	}
	
//	void OnSlaveEncoderAuxStep() override
//	{
//		console->info("Slave AUX step!");
//	}
	
	virtual void onStepsDone()
	{
		
		console->info( "Moved! Master: {0} Slave: {1}", GetMasterCurrentPosition(), GetSlaveCurrentPosition() );		
	}
	
	void SetTargetPosition(int32_t _TargetPosition)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		{
			TargetPosition = _TargetPosition;
		}
		mlock.unlock();
		cond_.notify_one();
	}
	
	void SetMasterCurrentPosition(int32_t _CurrentMasterPosition)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		{
			MasterCurrentPosition = _CurrentMasterPosition;
		}
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
	}
	
	void SetSlaveCurrentPosition(int32_t _CurrentSlavePosition)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		{
			SlaveCurrentPosition = _CurrentSlavePosition;
		}
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
	}
};

VerticalMovement g_Vertical;

#endif //RPIFIRMWARE_VERTICALMOVEMENT_H
