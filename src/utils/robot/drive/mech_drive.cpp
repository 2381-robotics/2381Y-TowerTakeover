#include "utils/robot/drive/mech_drive.h"
#include "api.h"
Mech_Drive::Mech_Drive(Motor_Controller* left_front_motor_controller, Motor_Controller* left_back_motor_controller, Motor_Controller* right_front_motor_controller, Motor_Controller* right_back_motor_controller){
  _left_front_motor_controller = left_front_motor_controller;
  _left_back_motor_controller = left_back_motor_controller;
  _right_front_motor_controller = right_front_motor_controller;
  _right_back_motor_controller = right_back_motor_controller;
}
void Mech_Drive::Drive(double left_x, double left_y, double right_x, double right_y){
  _left_back_setpoint = (left_y - left_x + abs(right_x)*(right_x)/127);
  _left_front_setpoint = (left_y + left_x + abs(right_x)*(right_x)/127);
  _right_back_setpoint = (left_y + left_x - abs(right_x)*(right_x)/127);
  _right_front_setpoint = (left_y - left_x  - abs(right_x)*(right_x)/127);

  pros::lcd::set_text(0+4, "left_back_motor:" + std::to_string((_left_back_setpoint)));

  pros::lcd::set_text(2+4, "left_front_motor:" + std::to_string((_left_front_setpoint)));

  pros::lcd::set_text(1+4, "right_back_motor:" + std::to_string((_right_back_setpoint)));

  pros::lcd::set_text(3+4, "right_front_motor:" + std::to_string((_right_front_setpoint)));

  pros::lcd::set_text(0, "left_back_motor:" + std::to_string(_left_back_motor_controller->Set_Speed(_left_back_setpoint)));

  pros::lcd::set_text(2, "left_front_motor:" + std::to_string( _left_front_motor_controller->Set_Speed(_left_front_setpoint)));

  pros::lcd::set_text(1, "right_back_motor:" + std::to_string(_right_back_motor_controller->Set_Speed(_right_back_setpoint)));

  pros::lcd::set_text(3, "right_front_motor:" + std::to_string(  _right_front_motor_controller->Set_Speed(_right_front_setpoint)));
  // _left_back_motor_controller->Set_Speed(_left_back_setpoint);
  // _left_front_motor_controller->Set_Speed(_left_front_setpoint);
  // _right_back_motor_controller->Set_Speed(_right_back_setpoint);
  // _right_front_motor_controller->Set_Speed(_right_front_setpoint);
}
