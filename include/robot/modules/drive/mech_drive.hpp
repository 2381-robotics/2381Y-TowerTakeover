#ifndef MECH_DRIVE_HPP
#define MECH_DRIVE_HPP

#include "robot/modules/drive/drive.hpp"
#include "robot/control/motor_controller.hpp"
#include "robot/control/distance_controller.hpp"
#include <array>

class Mech_Drive : public Drive {
public:
  Mech_Drive();

  void Set_Drive(double left_x, double left_y, double right_x, double right_y) override;
  void Set_Point_Drive(double speed, double direction, double distance, double turnDirection = 0, double accelScaling = 1) override;
  
  double Get_Speed() override;
  double Get_Distance() override;

  void Set_Init_Point() override;
  std::array<double, 4> initial_position = {0,0,0,0};
  void Reset_Point() override;
  double test;
  bool get_running() override;
  void Set_Turn(double Speed, double direction, double distance) override;
  std::array<double, 2> Convert(double speed = 0, double direction = 0);
  std::string name = "Mech Drive";

  std::array<double,3> distance_controller_pid_values = {1.5, 0.0015 ,0};
  void Stop() override;

  Pid* Drive_Distance_Controller;

  double _point_drive_distance;
  bool _is_running = false;

  enum Motor_Ref {left_back = 0, left_front, right_back, right_front};


protected:
  void Move_Motor() override;

  std::array<double,4> _pid_inputs = {0,0,0,0};

  Motor_Controller *_left_front_motor_controller;
  Motor_Controller *_left_back_motor_controller;
  Motor_Controller *_right_front_motor_controller;
  Motor_Controller *_right_back_motor_controller;
  Pid *_master_pid;
  double _left_front_setpoint;
  double _left_back_setpoint;
  double _right_front_setpoint;
  double _right_back_setpoint;

  void Create() override;

  double _master_offset = 0;
  double lboffset= 0, rboffset= 0, rfoffset = 0, lfoffset = 0;
  double lbDistance = 0, rbDistance = 0, rfDistance = 0, lfDistance = 0, masterDistance = 0;
  // double lboffset = 0, rboffset = 0, rfoffset = 0, lfoffset = 0;

  double _left_back_motor_value, _left_front_motor_value, _right_back_motor_value, _right_front_motor_value;

  double _motor_value_average;
  double _master_setpoint;
  double _previous_setpoint;
  double _master_error_average;

  void Module_Move();

};
#endif
