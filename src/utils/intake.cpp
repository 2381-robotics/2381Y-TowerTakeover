#include "intake.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>




double Intake::Get_Speed() {
  return (leftIntakeController->Get_Speed()+rightIntakeController->Get_Speed())/2;
}

void Intake::Set_Intake(double upL, double downL)
{
    _intake_setpoint = (upL * 80);
    _outtake_setpoint = (downL * 80);

    // pros::lcd::set_text(0, "old master setpoint:" + std::to_string((_master_intake_setpoint)));
    _master_intake_setpoint = (_intake_setpoint + _outtake_setpoint);
    // pros::lcd::set_text(1, "new master setpoint:" + std::to_string((_master_intake_setpoint)));

    double intake_tuning_coefficient = _master_intake_pid->Update(0, _master_intake_error_average);
    // pros::lcd::set_text(2, "tuning coeff:" + std::to_string(tuning_coefficient));


    _left_intake_value = leftIntakeController->Set_Speed(_master_intake_setpoint * intake_tuning_coefficient);
    _right_intake_value = rightIntakeController->Set_Speed(_master_intake_setpoint * intake_tuning_coefficient);
     _intake_value_average = (_left_intake_value + _right_intake_value) / 2;

    // pros::lcd::set_text(3, "motor value average:" + std::to_string((_intake_value_average)));

    if (_master_intake_setpoint >= 0)
    {
        _master_intake_error_average = _intake_value_average - _master_intake_setpoint;
    }
    else
    {
        _master_intake_error_average = _master_intake_setpoint - _intake_value_average;
    }

    // pros::lcd::set_text(4, "master error average:" + std::to_string((_master_intake_error_average)));
}
//Empty default constructor for blank factory arguments.
Intake::Intake(){};
void Intake::create()
{
    Motor_Controller *leftIntakeController = new Motor_Controller(&pid_intake_left_values[0], &pid_intake_left_values[1], &pid_intake_left_values[2], &intakeMotorLeft);
    Motor_Controller *rightIntakeController = new Motor_Controller(&pid_intake_right_values[0], &pid_intake_right_values[1], &pid_intake_right_values[2], &intakeMotorRight);

    _master_intake_pid = new Pid(&(master_intake_pid_values)[0], &(master_intake_pid_values)[1], &(master_intake_pid_values)[2]);
    _master_intake_error_average = 0;
    _master_intake_setpoint = 1;
}
