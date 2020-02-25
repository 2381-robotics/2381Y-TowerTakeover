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
        pros::lcd::set_text(0, "hi");
    }
    else {
        intakeMotorLeft.move_voltage(_pid_inputs[Left] / 127 * 12000);
        intakeMotorRight.move_voltage(_pid_inputs[Right] / 127 * 12000);
    }

    // _intake_value_average = (_left_intake_value + _right_intake_value) / 2;

    
    // if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
    // {
    //     string s = to_string(intakeMotorLeft.get_temperature());
    //     char cstr[s.size() + 1];
    //     std::copy(s.begin(), s.end(), cstr);
    //     cstr[s.size()] = '\0';
    // }
}

// double Intake::Get_Real_Target()
// {
//   double real_target = _target_height;
//   if(_target_height - _previous_target > _max_intake_speed) {
//      real_target = _previous_target + _max_intake_speed;
//   }
//   else if (_previous_target - _target_height > _max_intake_speed)
//   {
//     real_target = _previous_target - _max_intake_speed;
//   }
//   _previous_target = real_target;
//   return real_target;
// }


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

    // _outtake_setpoint = (downL * 80);
    // pros::lcd::set_text(1, "new master setpoint:" + std::to_string((_master_intake_setpoint)));

    double intake_tuning_coefficient = _master_intake_pid->Update(0, _master_intake_error_average);
    // pros::lcd::set_text(2, "tuning coeff:" + std::to_string(intake_tuning_coefficient));
    _pid_inputs[Left] = _intake_setpoint;
    _pid_inputs[Right] = _intake_setpoint;

}
//Empty default constructor for blank factory arguments.
Intake::Intake(){};

void Intake::Reset() {
    Stop();
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
