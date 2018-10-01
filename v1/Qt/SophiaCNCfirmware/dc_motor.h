#ifndef DC_MOTOR_H
#define DC_MOTOR_H

/* DRIVER: MD13S (13A) */

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QTime>

#include "hal.h"

#define DC_MOTOR_PWM_PERIOD_MS  10

enum class DCMotorDirection
{
    BACKWARD,
    FORWARD
};

enum class DCMotorState
{
    STOPPED,
    RUNNING
};

class DCMotor : public QObject
{
    Q_OBJECT
private:
    int EnablePin, DirectionPin;
    DCMotorState CurrentState = DCMotorState::STOPPED;
    int CurrentSpeed = 50;

    void PWM(bool enabled, int _duty = 0);

public:
    DCMotor(int _enable_pin, int _direction);
    void SetDirection(DCMotorDirection new_direction);
    void Run(int speed_procents);
    void Stop();
    bool IsRunning();
    static long map(long, long, long, long, long);
};

#endif // DC_MOTOR_H
