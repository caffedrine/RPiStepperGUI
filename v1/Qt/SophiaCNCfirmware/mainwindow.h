#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMessageBox>
#include <QTimer>
#include <QMainWindow>
#include <QThread>
#include <QTime>

#include "hal.h"
#include "PushButton.h"
#include "stepper.h"
#include "dc_motor.h"

namespace Ui
{
class MainWindow;
}

typedef enum States
{
    EMERGENCY_STOP_REQUEST,
    STANDBY,
    INIT_MOTORS,
    WAIT_INIT,

    MOVE_STEPPER,
    WAIT_MOVE_STEPPER,

    WAIT_SECOND_START,
    BLOCK_CURTAIN,
    WAIT_BLOCK_CURTAIN,
    TENSION_CURTAIN,
    WAIT_TENSION_CURTAIN,
    START_CUTTER_MOTOR,
    WAIT_START_CUTTER_MOTOR,
    CUT_CURTAIN,
    WAIT_CUT_CURTAIN,
    STOP_CUTTER_MOTOR,
    WAIT_STOP_CUTTER_MOTOR,
    MOTORS_RETURN,
    WAIT_MOTOR_RETURN,

    STATES_NUMBER
}states_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString StatesDescrpiption[STATES_NUMBER];

private:
    void update_digits(int value);
    int get_digits_value();
    void set_digits_value(int new_val);

private slots:
    void on_pushButton_digit1_pressed();
    void on_pushButton_digit2_pressed();
    void on_pushButton_digit3_pressed();
    void on_pushButton_digit4_pressed();
    void on_pushButton_digit5_pressed();
    void on_pushButton_digit6_pressed();
    void on_pushButton_digit7_pressed();
    void on_pushButton_digit8_pressed();
    void on_pushButton_digit9_pressed();
    void on_pushButton_digit0_pressed();

    void on_pushButton_up_pressed();
    void on_pushButton_up_released();
    void on_pushButton_left_pressed();
    void on_pushButton_left_released();
    void on_pushButton_right_pressed();
    void on_pushButton_right_released();
    void on_pushButton_down_pressed();
    void on_pushButton_down_released();

    void on_pushButton_start_pressed();
    void on_pushButton_stop_pressed();
    void on_pushButton_stop_released();
    void on_pushButton_start_released();

    void on_verticalMotorInitSwitch_Pressed();
    void on_verticalMotorInitSwitch_Released();
    void on_horizontalMotorInitSwitch_Pressed();
    void on_horizontalMotorInitSwitch_Released();
    void on_cropFinishedSensor_Pressed();
    void on_cropFinishedSensor_Released();

    void on_pushButton_digit1_released();
    void on_pushButton_digit2_released();
    void on_pushButton_digit3_released();
    void on_pushButton_digit4_released();
    void on_pushButton_digit5_released();
    void on_pushButton_digit6_released();
    void on_pushButton_digit7_released();
    void on_pushButton_digit8_released();
    void on_pushButton_digit9_released();
    void on_pushButton_digit0_released();
    void on_pushButton_value3_toggled(bool checked);
    void on_pushButton_value2_toggled(bool checked);
    void on_pushButton_value1_toggled(bool checked);
    void on_pushButton_value0_toggled(bool checked);
    void on_pushButton_reinitialize_pressed();

    void TickTask();
    void StatesMachine();

    void on_pushButton_reinitialize_released();
    void on_stepperPositionUpdated(float new_position);

    void on_pushButton_AutomaticManual_pressed();
    void on_pushButton_Cutter_toggled(bool checked);
    void on_pushButton_Valves_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    class MainWorkerThread *StatesMachineThread = Q_NULLPTR;
    class TickingThread *TickThread = Q_NULLPTR;

    PushButton *HorizontalMotorInitButton = Q_NULLPTR;
    PushButton *VerticalMotorInitButton = Q_NULLPTR;
    PushButton *CropFinishButton = Q_NULLPTR;
    Stepper *stepper = Q_NULLPTR;
    DCMotor *dc_motor = Q_NULLPTR;

    states_t CurrentState = STANDBY, PreviousState = STANDBY;
    int CurrentPosition = 0;

    QTime WaitTimer;
    void SetState(states_t state);

    bool ActionStarted = false;
    bool SystemInitialized = true;
    int StartPhase = 1;
};

/** ********************************************** **/

class MainWorkerThread : public QThread
{
    Q_OBJECT
private:
public:
    void run()
    {
        while(1)
        {
            emit Tick();
            QThread::msleep(1);
        }
    }
signals:
  void Tick();
};

/** ********************************************** **/


class TickingThread : public QThread
{
    Q_OBJECT
private:
public:
    void run()
    {
        while(1)
        {
            emit Tick();
            QThread::msleep ( 1 );
        }
    }

signals:
  void Tick();
};

#endif // MAINWINDOW_H
