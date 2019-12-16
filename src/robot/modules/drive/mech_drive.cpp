#include "robot/modules/drive/mech_drive.hpp"
#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>


double Mech_Drive::Get_Speed() {
  return (_left_front_motor_controller->Get_Speed()+_left_back_motor_controller->Get_Speed()+ _right_back_motor_controller->Get_Speed() + _right_front_motor_controller->Get_Speed())/4;
}

void Mech_Drive::Set_Init_Point() {
  std::array <double, 4> tempArr = {_left_front_motor_controller->Get_Distance(), _left_back_motor_controller->Get_Distance(), _right_front_motor_controller->Get_Distance(), _right_back_motor_controller->Get_Distance()};
  initial_position = tempArr;
}
bool Mech_Drive::get_running() {
    return _is_running;
}
void Mech_Drive::Reset_Point()
{
  Set_Init_Point();
  _is_running = true;
}
double Mech_Drive::Get_Distance() {
  return (std::abs(_left_front_motor_controller->Get_Distance() - initial_position[0]) + std::abs(_left_back_motor_controller->Get_Distance() - initial_position[1]) + std::abs(_right_front_motor_controller->Get_Distance() - initial_position[2]) + std::abs(_right_back_motor_controller->Get_Distance() - initial_position[3]))/ 4;
}

void Mech_Drive::Set_Drive(double left_x, double left_y, double right_x, double right_y){
  // pros::lcd::set_text(1, _left_back_motor_controller->Get_Speed);
  _left_back_setpoint = (left_y - left_x + std::abs(right_x)*(right_x)/127);
  _left_front_setpoint = (left_y + left_x + std::abs(right_x)*(right_x)/127);
  _right_back_setpoint = (left_y + left_x - std::abs(right_x)*(right_x)/127);
  _right_front_setpoint = (left_y - left_x  - std::abs(right_x)*(right_x)/127);

  _master_setpoint = (_left_back_setpoint + _left_front_setpoint + _right_back_setpoint + _right_front_setpoint)/4;

  double tuning_coefficient = _master_pid->Update(0, _master_error_average);

  _left_back_motor_value = _left_back_motor_controller->Set_Speed(_left_back_setpoint *tuning_coefficient);
  _left_front_motor_value = _left_front_motor_controller->Set_Speed(_left_front_setpoint * tuning_coefficient);
  _right_back_motor_value = _right_back_motor_controller->Set_Speed(_right_back_setpoint * tuning_coefficient);
  _right_front_motor_value = _right_front_motor_controller->Set_Speed(_right_front_setpoint * tuning_coefficient);

  _motor_value_average = (_left_back_motor_value + _left_front_motor_value +  _right_back_motor_value +  _right_front_motor_value)/4;

  if(_master_setpoint >= 0){
    _master_error_average = _motor_value_average - _master_setpoint;
  } else{
    _master_error_average = _master_setpoint - _motor_value_average;
  }

}


std::array<double, 2> Mech_Drive::Convert(double speed, double direction){
  double x = cos(direction* (3.14159/180)) *speed;
  double y = sin(direction * (3.14159/180)) * speed;
  std::array<double, 2> drive_coordinates = {x, y};
  return drive_coordinates;
}

void Mech_Drive::Set_Turn(double speed, double direction, double distance)
{
  std::array<double, 2> drive_convert = Convert(speed, direction);

  double actualDistance = this->Get_Distance();

  if (actualDistance < distance)
  {
    Set_Drive(0, 0, speed, 0);
    _is_running = true;
  }
  else
  {
    Set_Drive(0, 0, 0, 0);
    _is_running = false;
  }
}
  void Mech_Drive:: Set_Point_Drive(double speed, double direction, double distance)
  {
    std::array<double, 2> drive_convert = Convert(speed, direction);
    pros::lcd::set_text(3, "Set Point Drive, Distance:"+  to_string(Get_Distance()));

    double actualDistance = this->Get_Distance();
    if (actualDistance < distance)
    {
      Set_Drive(drive_convert[1], (drive_convert)[0], 0, 0);
      _is_running = true;
  }
  else{
      Set_Drive(0, 0, 0, 0);
     _is_running = false;
}

}
void Mech_Drive::create()
{
  std::array<double, 4> tempArray = {0, 0, 0, 0};
  initial_position =  {0,0,0,0};
  _left_front_motor_controller = new Motor_Controller(&left_front_pid_values[0], &left_front_pid_values[1], &left_front_pid_values[2], &left_front_motor);
  _left_back_motor_controller = new Motor_Controller(&left_back_pid_values[0], &left_back_pid_values[1], &left_back_pid_values[2], &left_back_motor);
  _right_front_motor_controller = new Motor_Controller(&right_front_pid_values[0], &right_front_pid_values[1], &right_front_pid_values[2], &right_front_motor);
  _right_back_motor_controller = new Motor_Controller(&left_front_pid_values[0], &left_front_pid_values[1], &left_front_pid_values[2], &right_back_motor);
  _master_pid = new Pid(&(master_drive_pid_values)[0], &(master_drive_pid_values)[1], &(master_drive_pid_values)[2]);
  _master_error_average = 0;
  _master_setpoint = 1;
}
//Empty default constructor for blank factory arguments.
Mech_Drive::Mech_Drive(){

}


