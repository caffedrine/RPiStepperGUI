#include "stepper.h"

#include "hal.h"

Stepper::Stepper(int _directionPin, int _impulsPin)
{
    this->DirectionPin = _directionPin;
    this->PulsePin = _impulsPin;
    this->Init();
}

Stepper::Stepper(int _directionPin, int _impulsPin, int _enablePin)
{
    this->DirectionPin = _directionPin;
    this->PulsePin = _impulsPin;
    this->EnablePin = _enablePin;
    this->Init();
}

Stepper::Stepper(int _directionPin, int _impulsPin, int _enablePin, int _faultPin)
{
    this->DirectionPin = _directionPin;
    this->PulsePin = _impulsPin;
    this->EnablePin = _enablePin;
    this->FaultPin = _faultPin;
    this->Init();
}

Stepper::~Stepper()
{

}

void Stepper::Init()
{
    /* Init PWM frequency */
    gpioPWM(this->PulsePin, 0);
    gpioSetPWMfrequency(this->PulsePin, PULSES_PERIOD_US);

    /* bring motor into a disabled safe state */
    this->PWM(false);

    /* Set initial state */
    this->SetState(MotorState::STOPPED);

    /* Initialize worker thread */
    this->WorkThread = new MotorThread();
    connect(WorkThread, SIGNAL( Tick()), this, SLOT(Tick()) );
    WorkThread->start();
}

void Stepper::SetState(motor_state_t NewState)
{
    this->LastState = this->CurrentState;
    this->CurrentState = NewState;
}

void Stepper::PWM(bool enabled)
{
//    QProcess *process = new QProcess(this);
//    if(enabled)
//        process->start("bash", QStringList() << "-c" << "echo \"21=0.1\" > /dev/pi-blaster");
//    else
//        process->start("bash", QStringList() << "-c" << "echo \"21=0\" > /dev/pi-blaster");
//    process->waitForFinished();
    if(enabled)
         gpioPWM(this->PulsePin, 20);
    else
         gpioPWM(this->PulsePin, 0);
}

bool Stepper::IsRunning()
{
    if(CurrentState == MotorState::STOPPED)
        return false;
    return true;
}

void Stepper::Stop()
{
    if(!this->IsRunning())
        return;

    PWM(false);
    this->StepsToDo = 0;
    SetState(MotorState::STOPPED);
    CurrentStepsMade = (TimeCounter.elapsed()*1000/PULSES_PERIOD_US);
    if(Direction == FORWARD)
        SetPositionSteps( CurrentPositionSteps + CurrentStepsMade);
    else
        SetPositionSteps(CurrentPositionSteps - CurrentStepsMade);
}

void Stepper::Run()
{
    PWM(true);
    SetState(MotorState::RUNNING);
    TimeCounter.restart();
}

void Stepper::RunSteps(int steps)
{
    if(steps == 0)
        return;

    this->StepsToDo = steps;
    this->StepsDone = 0;
    PWM(true);
    SetState(MotorState::RUNNING_STEPS);
    TimeCounter.restart();
}

void Stepper::RunMillimeters(float millimeters)
{
    if(millimeters == 0)
    {
        if( this->IsRunning() )
            Stop();
        return;
    }

    /* Convert millimeters to steps */
    int steps = MmToSteps(millimeters);
    this->RunSteps(steps);
}

float Stepper::StepsToMm(int steps)
{
    return ( (float)((float)steps*(float)MILIMETERS_PER_REVOLUTION)/(float)STEPS_PER_REVOLUTION );
}

int Stepper::MmToSteps(float mm)
{
    return ( (mm*STEPS_PER_REVOLUTION)/MILIMETERS_PER_REVOLUTION );
}

bool Stepper::SetDirection(motor_direction_t NewDirection)
{
    this->Direction = NewDirection;
    return Vfb_WriteGpio( this->DirectionPin, (int)this->Direction );
}

float Stepper::GetLastDistance()
{
    return StepsToMm( CurrentStepsMade );
}

motor_direction_t Stepper::GetDirection()
{
    return this->Direction;
}

void Stepper::ResetPosition()
{
    //this->CurrentPositionSteps = STEPPER_POSITION_OFFSET;
    //this->LastPositionSteps = STEPPER_POSITION_OFFSET;
    this->SetPositionSteps(MmToSteps(STEPPER_POSITION_OFFSET));
}

void Stepper::SetPositionSteps(int new_position)
{
    this->LastPositionSteps = this->CurrentPositionSteps;
    this->CurrentPositionSteps = new_position;

    emit PositionChanged( StepsToMm( new_position ) );
}

void Stepper::Tick()
{
    if(CurrentState == MotorState::STOPPED)
    {
        Vfb_WriteGpio(this->PulsePin, LOW);
        return;
    }

    if(CurrentState == MotorState::RUNNING)
    {
        LastStepsMade = CurrentStepsMade;
        CurrentStepsMade = (TimeCounter.elapsed()*1000/PULSES_PERIOD_US);
        return;
    }

    if(CurrentState == MotorState::RUNNING_STEPS)
    {
        LastStepsMade = CurrentStepsMade;
        CurrentStepsMade = (TimeCounter.elapsed()*1000/PULSES_PERIOD_US);
        if(this->CurrentStepsMade >= this->StepsToDo - 8)
        {
            this->Stop();
            return;
        }
    }
}
