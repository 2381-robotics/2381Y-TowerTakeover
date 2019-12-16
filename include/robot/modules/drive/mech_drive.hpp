#ifndef MECH_DRIVE_HPP
#define MECH_DRIVE_HPP

#include "robot/modules/drive/drive.hpp"
#include "robot/control/motor_controller.hpp"
#include <array>

class Mech_Drive : public Drive {
public:
  Mech_Drive();

  void Set_Drive(double left_x, double left_y, double right_x, double right_y) override;
  void Set_Point_Drive(double speed, double direction, double distance) override;
  
  double Get_Speed() override;
  double Get_Distance() override;
  void create() override;

  void Set_Init_Point() override;
  std::array<double, 4> initial_position = {0,0,0,0};
  void Reset_Point() override;
  double test;
  bool get_running() override;
  void Set_Turn(double Speed, double direction, double distance) override;
  std::array<double, 2> Convert(double speed = 0, double direction = 0);
  std::string name = "Mech Drive";

  double _point_drive_distance;
  bool _is_running = false;

private:
  Motor_Controller *_left_front_motor_controller;
  Motor_Controller *_left_back_motor_controller;
  Motor_Controller *_right_front_motor_controller;
  Motor_Controller *_right_back_motor_controller;
  Pid *_master_pid;
  double _left_front_setpoint;
  double _left_back_setpoint;
  double _right_front_setpoint;
  double _right_back_setpoint;

  double _left_back_motor_value;
  double _left_front_motor_value;
  double _right_back_motor_value;
  double _right_front_motor_value;
  double _motor_value_average;
  double _master_setpoint;
  double _previous_setpoint;
  double _master_error_average;


};
#endif
