#include "intake.hpp"
#include "api.h"
#include "main.h"
#include "globals.hpp"
#include <array>


using namespace std;


double Intake::Get_Speed() {
  return (leftIntakeController->Get_Speed()+rightIntakeController->Get_Speed())/2;
}

void Intake::Move_Motor() {
    if(_pid_inputs[Left]+_pid_inputs[Right] == 0)
    {
        _left_intake_value = leftIntakeController->Set_Speed(_pid_inputs[Left]);
        _right_intake_value = rightIntakeController->Set_Speed(_pid_inputs[Right]);
        indexMotor.move_voltage(0);

    }
    else {
        intakeMotorLeft.move_voltage(_pid_inputs[Left] / 127 * 12000);
        intakeMotorRight.move_voltage(_pid_inputs[Right] / 127 * 12000);
    indexMotor.move_voltage(_pid_inputs[Left] / 127 * 12000);

    }
}
void Intake::Set_Intake(double intakeSpeed)
{

    _intake_setpoint = (intakeSpeed);
       if (_intake_setpoint >= 0)
    {
        _master_intake_error_average = _intake_value_average - _intake_setpoint;
    }
    else
    {
        _master_intake_error_average = _intake_setpoint - _intake_value_average;
    }
    double intake_tuning_coefficient = _master_intake_pid->Update(0, _master_intake_error_average);
    
    _pid_inputs[Left] = _intake_setpoint;
    _pid_inputs[Right] = _intake_setpoint;

}
//Empty default constructor for blank factory arguments.
Intake::Intake(){};

void Intake::Reset() {
    Stop();
}

double Intake::Get_Position()
{
    return (intakeMotorLeft.get_position() + intakeMotorRight.get_position()) / 2;
}
void Intake::Reset_Point()
{
    start_point = (intakeMotorLeft.get_position() + intakeMotorRight.get_position())/2;
}

void Intake::Index_Intake(double speed, double distance)
{
    if(abs(Get_Position()  - start_point)<  distance)
    {
        intake->Set_Intake(speed);
    }
    else {
        intake->Set_Intake(0);
    }

}

void Intake::Stop() {
    intakeMotorLeft.move_voltage(0);
    intakeMotorRight.move_voltage(0);
}

void Intake::Create()
{
    leftIntakeController = new Motor_Controller(&pid_intake_left_values[0], &pid_intake_left_values[1], &pid_intake_left_values[2], &intakeMotorLeft);
    rightIntakeController = new Motor_Controller(&pid_intake_right_values[0], &pid_intake_right_values[1], &pid_intake_right_values[2], &intakeMotorRight);

    _master_intake_pid = new Pid(&(master_intake_pid_values)[0], &(master_intake_pid_values)[1], &(master_intake_pid_values)[2]);
    _master_intake_error_average = 0;
}
