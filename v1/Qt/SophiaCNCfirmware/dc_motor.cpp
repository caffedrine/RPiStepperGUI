#include "dc_motor.h"

DCMotor::DCMotor(int _enable_pin, int _direction)
{
    this->EnablePin = _enable_pin;
    this->DirectionPin = _direction;

    Vfb_SetPinMode(this->EnablePin, OUTPUT);
    Vfb_SetPinMode(this->DirectionPin, OUTPUT);

    /* Make sure motor is disabled and in a safe state*/
    Vfb_WriteGpio(this->EnablePin, LOW);
    Vfb_WriteGpio(this->DirectionPin, HIGH);

    this->CurrentState = DCMotorState::STOPPED;

    /* Init PWM frequency */
    gpioPWM(this->EnablePin, 0);
    gpioSetPWMfrequency(this->EnablePin, 1000);
}

void DCMotor::SetDirection(DCMotorDirection new_direction)
{
   if(new_direction == DCMotorDirection::BACKWARD)
   {
       Vfb_WriteGpio(this->DirectionPin, HIGH);
   }
   else
   {
       Vfb_WriteGpio(this->DirectionPin, LOW);
   }
}

void DCMotor::Run(int speed_procents)
{
    this->CurrentSpeed = speed_procents;
    PWM(true, speed_procents);
}

void DCMotor::Stop()
{
    this->CurrentState = DCMotorState::STOPPED;
    this->PWM(false);
}

bool DCMotor::IsRunning()
{
    if(CurrentState == DCMotorState::STOPPED)
        return false;
    return true;
}

void DCMotor::PWM(bool enabled, int _duty)
{
//    float duty = (float)_duty/100.0;

//    QString dutyStr = QString::number( duty );

//    QProcess *process = new QProcess(this);
//    if(enabled)
//        process->start("bash", QStringList() << "-c" << "echo \"4=" + dutyStr + "\" > /dev/pi-blaster");
//    else
//        process->start("bash", QStringList() << "-c" << "echo \"4=0\" > /dev/pi-blaster");
//    process->waitForFinished();

    if(enabled)
         gpioPWM(this->EnablePin, DCMotor::map(_duty, 0, 100, 0, 255));
    else
         gpioPWM(this->EnablePin, 0);
}

long DCMotor::map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
