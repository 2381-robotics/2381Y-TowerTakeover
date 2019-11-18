#include "intake.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>


double Mech_Drive::Get_Speed()
{
    return (_left_front_motor_controller->Get_Speed() + _left_back_motor_controller->Get_Speed() + _right_back_motor_controller->Get_Speed() + _right_front_motor_controller->Get_Speed()) / 4;
}
void intake::Set_Drive(double upL, double downL)
{
    _intake_setpoint = (upL*80);
    _outtake_setpoint = (downL * -80);
    
    pros::lcd::set_text(0, "old master setpoint:" + std::to_string((_master_intake_setpoint)));
    _master_intake_setpoint = (_intake_setpoint + _outtake_setpoint);
    pros::lcd::set_text(1, "new master setpoint:" + std::to_string((_master_intake_setpoint)));

    double tuning_coefficient = _master_pid->Update(0, _master_intake_error_average);
    pros::lcd::set_text(2, "tuning coeff:" + std::to_string(tuning_coefficient));


    _left_intake_value = _left_back_motor_controller->Set_Speed(_intake_setpoint * tuning_coefficient);
    _right_intake_value = _right_back_motor_controller->Set_Speed(_right_back_setpoint * tuning_coefficient);
    _motor_intake_value_average = (_left_intake_value + _right_intake_value) / 2;

    pros::lcd::set_text(3, "motor value average:" + std::to_string((_motor_intake_value_average)));

    if (_master_intake_setpoint >= 0)
    {
        _master_intake_error_average = _motor_intake_value_average - _master_intake_setpoint;
    }
    else
    {
        _master_intake_error_average = _master_intake_setpoint - _motor_intake_value_average;
    }

    pros::lcd::set_text(4, "master error average:" + std::to_string((_master_intake_error_average)));
}
//Empty default constructor for blank factory arguments.
void intake::create()
{
    Motor_Controller *leftIntakeController = new Motor_Controller(&pidValues[0], &pidValues[1], &pidValues[2], &intakeMotorLeft);
    Motor_Controller *rightIntakeController = new Motor_Controller(&pidValues[0], &pidValues[1], &pidValues[2], &intakeMotorRight);

    _master_intake_pid = new Pid(&(master_drive_pid_values)[0], &(master_drive_pid_values)[1], &(master_drive_pid_values)[2]);
    _master_intake_error_average = 0;
    _master_intake_setpoint = 1;
}
