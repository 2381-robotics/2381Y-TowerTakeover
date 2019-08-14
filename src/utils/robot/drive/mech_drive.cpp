#include "utils/robot/drive/mech_drive.h"

Mech_Drive::Mech_Drive(Motor_Controller* left_front_motor_controller, Motor_Controller* left_back_motor_controller, Motor_Controller* right_front_motor_controller, Motor_Controller* right_back_motor_controller){
  _left_front_motor_controller = left_front_motor_controller;
  _left_back_motor_controller = left_back_motor_controller;
  _right_front_motor_controller = right_front_motor_controller;
  _right_back_motor_controller = right_back_motor_controller;
}
void Mech_Drive::Drive(double left_x, double left_y, double right_x, double right_y){
  _left_back_setpoint = (left_y - left_x + right_x*(right_x)/127);
  _left_front_setpoint = (left_y + left_x + right_x*(right_x)/127);
  _right_back_setpoint = (left_y + left_x - right_x*(right_x)/127);
  _right_front_setpoint = (left_y - left_x  - right_x*(right_x)/127);
  
  _left_back_motor_controller->Set_Speed(_left_back_setpoint);
  _left_front_motor_controller->Set_Speed(_left_front_setpoint);
  _right_back_motor_controller->Set_Speed(_right_back_setpoint);
  _right_front_motor_controller->Set_Speed(_right_front_setpoint);
}
