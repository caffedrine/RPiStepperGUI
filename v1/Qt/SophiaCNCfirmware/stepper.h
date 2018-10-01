#ifndef STEPPER_H
#define STEPPER_H

#include <QThread>
#include <QProcess>
#include <QTime>
#include <QFile>

#define PULSES_PERIOD_US            1000 /* 1ms */
#define STEPS_PER_REVOLUTION        400
#define MILIMETERS_PER_REVOLUTION   62.45f

#define STEPPER_POSITION_OFFSET     155

typedef enum MotorDirection
{
    FORWARD  = 1,
    BACKWARD = 0
}motor_direction_t;

typedef enum MotorState
{
    STOPPED,
    RUNNING,
    RUNNING_STEPS
}motor_state_t;

class Stepper : public QObject
{
    Q_OBJECT
public:
    Stepper(int directionPin, int impulsPin);
    Stepper(int directionPin, int impulsPin, int enablePin);
    Stepper(int directionPin, int impulsPin, int enablePin, int faultPin);
    ~Stepper();

    int StepsToDo = 0, StepsDone  = 0;
    int CurrentStepsMade = 0, LastStepsMade = 0;
    int CurrentPositionSteps = 0, LastPositionSteps = 0;

    void Stop();
    void Run();
    void RunSteps(int steps);
    void RunMillimeters(float millimeters);
    bool SetDirection(motor_direction_t NewDirection);
    bool IsRunning();

    motor_direction_t GetDirection();
    float GetLastDistance();
    void ResetPosition();

    float StepsToMm(int steps);
    int MmToSteps(float mm);

private:
    int DirectionPin, PulsePin, EnablePin, FaultPin;

    motor_direction_t Direction = MotorDirection::FORWARD;
    motor_state_t CurrentState = MotorState::STOPPED, LastState = MotorState::STOPPED;

    QTime TimeCounter;
    class MotorThread *WorkThread = Q_NULLPTR;

    void Init();
    void SetState(motor_state_t NewState);
    void Step();
    void PWM(bool enabled);
    void SetPositionSteps(int new_position);

private slots:
    void Tick();

signals:
    void PositionChanged(float new_position);
};

class MotorThread : public QThread
{
    Q_OBJECT
private:
public:
    void run()
    {
        while(1)
        {
            emit Tick();
            QThread::msleep( 1 );
        }
    }

signals:
  void Tick();
};

#endif // STEPPER_H
