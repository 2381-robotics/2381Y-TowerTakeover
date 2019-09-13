#include "utils/robot/drive/mech_drive.hpp"
#include "../../motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>

Mech_Drive::Mech_Drive() : test{left_front_pid_values[0]}{
pros::Motor something (LEFT_BACK_MOTOR_PORT);
  _left_front_motor_controller = new Motor_Controller(&left_front_pid_values[0], &left_front_pid_values[1], &left_front_pid_values[2], &left_front_motor);
  _left_back_motor_controller = new Motor_Controller(&left_back_pid_values[0], &left_back_pid_values[1], &left_back_pid_values[2], &left_back_motor);
  _right_front_motor_controller = new Motor_Controller(&right_front_pid_values[0], &right_front_pid_values[1], &right_front_pid_values[2], &right_front_motor);
  _right_back_motor_controller = new Motor_Controller(&left_front_pid_values[0], &left_front_pid_values[1], &left_front_pid_values[2], &left_front_motor);

  _master_pid = new Pid( &(master_pid_values)[0], &(master_pid_values)[1], &(master_pid_values)[2]);
   _master_error_average = 0;
   _master_setpoint = 1;
  test = (master_pid_values)[0];

}
double Mech_Drive::Get_Speed() {
  return (_left_front_motor_controller->Get_Speed()+_left_back_motor_controller->Get_Speed()+ _right_back_motor_controller->Get_Speed() + _right_front_motor_controller->Get_Speed())/4;
}
void Mech_Drive::Set_Drive(double left_x, double left_y, double right_x, double right_y){
  _left_back_setpoint = (left_y - left_x + std::abs(right_x)*(right_x)/127);
  _left_front_setpoint = (left_y + left_x + std::abs(right_x)*(right_x)/127);
  _right_back_setpoint = (left_y + left_x - std::abs(right_x)*(right_x)/127);
  _right_front_setpoint = (left_y - left_x  - std::abs(right_x)*(right_x)/127);
  pros::lcd::set_text(0, "old master setpoint:" + std::to_string((_master_setpoint)));
  _master_setpoint = (_left_back_setpoint + _left_front_setpoint + _right_back_setpoint + _right_front_setpoint)/4;
  pros::lcd::set_text(1, "new master setpoint:" + std::to_string((_master_setpoint)));

  double tuning_coefficient = _master_pid->Update(0, _master_error_average);
  pros::lcd::set_text(2, "tuning coeff:" + std::to_string(tuning_coefficient));


  // pros::lcd::set_text(0+4, "left_back_motor:" + std::to_string((_left_back_setpoint)));
  //
  // pros::lcd::set_text(2+4, "left_front_motor:" + std::to_string((_left_front_setpoint)));
  //
  // pros::lcd::set_text(1+4, "right_back_motor:" + std::to_string((_right_back_setpoint)));
  //
  // pros::lcd::set_text(3+4, "right_front_motor:" + std::to_string((_right_front_setpoint)));


  // pros::lcd::set_text(0, "left_back_motor:" + std::to_string(_left_back_motor_controller->Set_Speed(_left_back_setpoint)));
  //
  // pros::lcd::set_text(2, "left_front_motor:" + std::to_string( _left_front_motor_controller->Set_Speed(_left_front_setpoint)));
  //
  // pros::lcd::set_text(1, "right_back_motor:" + std::to_string(_right_back_motor_controller->Set_Speed(_right_back_setpoint)));
  //
  // pros::lcd::set_text(3, "right_front_motor:" + std::to_string(  _right_front_motor_controller->Set_Speed(_right_front_setpoint)));
  _left_back_motor_value = _left_back_motor_controller->Set_Speed(_left_back_setpoint * tuning_coefficient);
  _left_front_motor_value = _left_front_motor_controller->Set_Speed(_left_front_setpoint * tuning_coefficient);
  _right_back_motor_value = _right_back_motor_controller->Set_Speed(_right_back_setpoint * tuning_coefficient);
  _right_front_motor_value = _right_front_motor_controller->Set_Speed(_right_front_setpoint * tuning_coefficient);
  _motor_value_average = (_left_back_motor_value + _left_front_motor_value +  _right_back_motor_value +  _right_front_motor_value)/4;
  pros::lcd::set_text(3, "motor value average:" + std::to_string((_motor_value_average)));
  if(_master_setpoint >= 0){
    _master_error_average = _motor_value_average - _master_setpoint;
  } else{
    _master_error_average = _master_setpoint - _motor_value_average;
  }

  pros::lcd::set_text(4, "master error average:" + std::to_string((_master_error_average)));

}

void Mech_Drive::create() {}
