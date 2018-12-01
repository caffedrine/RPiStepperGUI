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
#include "peripherals/SlaveEncoder.h"
#include "peripherals/MasterDC.h"
#include "peripherals/SlaveDC.h"
#include "peripherals/SensorVerticalMaster.h"
#include "peripherals/SensorVerticalSlave.h"

class VerticalMovement : protected MasterEncoder, protected SlaveEncoder
{
public:
	explicit VerticalMovement() : MasterEncoder(ENCODER_MASTER_GPIO), SlaveEncoder(ENCODER_SLAVE_GPIO)
	{
		BackgroundWorker = std::thread([this]()
									   {
										   BackgroundWork();
									   });
		BackgroundWorker.detach();
	}
	
	uint32_t GetCurrentPosition()
	{
		return GetMasterCurrentPosition();
	}
	
	uint32_t GetTargetPosition()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		uint32_t tmp = TargetPosition;
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
		return tmp;
	}
	
	bool IsRunning()
	{
		if( (GetMasterCurrentPosition() != GetTargetPosition()) || _IsResetting )
			return true;
		return false;
	}
	
	float GetCurrPositionMM()
	{
		return Enc2Mm( GetCurrentPosition() );
	}
	
	void MoveToMM(uint32_t mm)
	{
		MoveTo(Mm2Enc(mm));
	}
	
	void MoveTo(uint32_t encoder_units)
	{
		SetTargetPosition( encoder_units );
	}
	
	void MoveUp()
	{
		MoveTo( MAX_POSITION );
	}
	
	void MoveDown()
	{
		MoveTo( 0 );
	}
	
	void Stop()
	{
		/*  Stop */
		SetTargetPosition( GetMasterCurrentPosition() );
	}
	
	void Reset()
	{
		this->_IsResetting = true;
	}
	
	float Enc2Mm(uint32_t enc)
	{
		return ((float) MasterCurrentPosition * MM_PER_STEP);
	}
	
	uint32_t Mm2Enc(float mm)
	{
		return (uint32_t) (mm / MM_PER_STEP);
	}
	
	void Tick()
	{
	
	}

private:
	const float MM_PER_STEP = 1.5;
	const float OFFSET = 10;
	const float MIN_POSITION = OFFSET;
	const float MAX_POSITION = 5000;
	
	/* PID constants */
	const double DT = 0.01;    // loop interval time
	const double MAX = 255;        // maximum value of manipulated variable
	const double MIN = 1;        // minimum value of manipulated variable
	const double KP = 0.1;        // proportional gain
	const double KD = 0.01;        // derivative gain
	const double KI = 0.5;        // Integral gain
	PID pid = PID(DT, MAX, MIN, KP, KD, KI);
	
	/* Store positions */
	uint32_t MasterCurrentPosition = 0, TargetPosition = 0;
	/* Slave motor position */
	uint32_t SlaveCurrentPosition = 0;
	
	/* Safe read/write positions */
	std::mutex mutex_;
	std::condition_variable cond_;
	
	/* Background thread */
	std::thread BackgroundWorker;
	
	/* Flag when the vertical motors are resetting are*/
	bool _IsResetting = false;
	
	void BackgroundWork()
	{
		while( true )
		{
			if( !_ProgramContinue )
				break;
			
			if(_IsResetting)
			{
				if(g_SensorVerticalMaster.CurrentState == PushButtonState::DOWN)
				{
					g_MasterDC.Stop();
				}
				else
				{
					g_MasterDC.SetDirection(MotorDcDirection::BACKWARD);
					g_MasterDC.SetSpeed(DC_MOTOR_DEFAULT_SPEED);
					g_MasterDC.Run();
				}
				
				if(g_SensorVerticalSlave.CurrentState ==  PushButtonState::DOWN)
				{
					g_SlaveDC.Stop();
				}
				else
				{
					g_SlaveDC.SetDirection(MotorDcDirection::BACKWARD);
					g_SlaveDC.SetSpeed(DC_MOTOR_DEFAULT_SPEED);
					g_SlaveDC.Run();
				}
				
				if(g_SensorVerticalMaster.CurrentState == PushButtonState::DOWN && g_SensorVerticalSlave.CurrentState == PushButtonState::DOWN )
				{
					g_MasterDC.Stop();
					g_MasterDC.Stop();
					/* Optionally reset encoders */
					MasterEncoder::Reset();
					SlaveEncoder::Reset();
					_IsResetting = false;
					/* Move motors to init buttons then stop */
					SetMasterCurrentPosition(0);
					SetSlaveCurrentPosition(0);
					SetTargetPosition(0);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}
			
			if(GetMasterCurrentPosition() > GetTargetPosition())
			{
				/* Move down */
				g_MasterDC.SetDirection(MotorDcDirection::BACKWARD);
				g_MasterDC.Run();
			}
			else if(GetMasterCurrentPosition() < GetTargetPosition())
			{
				/* Move up */
				g_MasterDC.SetDirection(MotorDcDirection::FORWARD);
				g_MasterDC.Run();
			}
			else if(GetMasterCurrentPosition() == GetTargetPosition())
			{
				/* Motor already in target potition */
				g_MasterDC.Stop();
			}
			
			HandleSlave();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	
	void HandleSlave()
	{
		/**
		 *  Slave have the task to monitor master's position and stick to it's position
		 */
		
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
		if(GetSlaveCurrentPosition() == GetMasterCurrentPosition())
		{
			g_SlaveDC.Stop();
		}
		else
		{
			double NewSpeed = pid.calculate(MasterCurrentPosition, SlaveCurrentPosition);
			//g_SlaveDC.SetSpeed((uint8_t) NewSpeed);
			g_SlaveDC.Run();
		}
	}

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
		console->info("Master current position: {}", GetCurrentPosition());
	}
	
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
		console->info("Slave current position: {}", this->SlaveCurrentPosition);
	}
	
	virtual void onStepsDone()
	{
		console->info("Steps finished!");
	}
	
	void SetTargetPosition(uint32_t _TargetPosition)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		{
			TargetPosition = _TargetPosition;
		}
		mlock.unlock();
		cond_.notify_one();
	}
	
	void SetMasterCurrentPosition(uint32_t _CurrentMasterPosition)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		{
			MasterCurrentPosition = _CurrentMasterPosition;
		}
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
	}
	
	void SetSlaveCurrentPosition(uint32_t _CurrentSlavePosition)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		{
			SlaveCurrentPosition = _CurrentSlavePosition;
		}
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
	}
	
	uint32_t GetMasterCurrentPosition()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		uint32_t tmp = MasterCurrentPosition;
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
		return tmp;
	}
	
	uint32_t GetSlaveCurrentPosition()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		uint32_t tmp = SlaveCurrentPosition;
		mlock.unlock();     // unlock before notificiation to minimize mutex con
		cond_.notify_one(); // notify one waiting thread
		return tmp;
	}
};

VerticalMovement g_Vertical;

#endif //RPIFIRMWARE_VERTICALMOVEMENT_H
