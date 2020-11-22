#ifndef MECH_DRIVE_HPP
#define MECH_DRIVE_HPP

#include "robot/modules/drive/drive.hpp"
#include "robot/control/motor_controller.hpp"
#include "robot/control/distance_controller.hpp"
#include <array>
#include <complex>

class Mech_Drive : public Drive {
public:
  Mech_Drive();

  void Set_Drive(double left_x, double left_y, double right_x, double right_y) override;
  void Set_Drive(double left_x, double left_y, double right_x, double right_y, bool wallAlign);

  void Set_Point_Drive(double speed, double direction, double distance, double turnSpeed = 0, double accelSpeed =  1, double deaccelSpeed = 1,  bool wallAlign = false, double criticalPoint = 400, double criticalMultiplier = 1, std::array<double, 4> endVelo = {0, 0, 0, 0});
  void Set_Path_Drive(std::complex<double> EndPoint, double accelSpeed =  1, double deaccelSpeed = 1, double criticalPoint = 400, double criticalMultiplier = 1, std::array<double, 4> endVelo = {0, 0, 0, 0});
  
  void Set_Curve_Drive(std::complex<double> EndPoint, double EndAngle, std::array<double, 2> speed, std::array<double, 2> errorTolerance);
  void Set_Curve_Drive(std::complex<double> EndPoint, double EndAngle, double speed = 100, std::array<double, 2> errorTolerance = {1,1});
  void Set_Curve_Drive(std::complex<double> EndPoint, double EndAngle, std::array<double, 2> speed, double errorTolerance = 1);
  void Set_Curve_Drive(std::complex<double> EndPoint, double EndAngle, double speed, double errorTolerance);



  std::array<double,4> previousVelo;

  int started;
  std::array<double,4> unstartedArray();

  double Get_Speed() override;
  double Get_Distance() override;

  void Set_Path();
  void Follow_Path();

  std::array<double,2> lookaheadPoint = {0,0};


  void Move_Wheel(double Speed) override; 
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

  void Create() override;

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


  // double _master_offset = 2550;
  double _master_offset = 1;

  // double lboffset = 2550, rboffset = 2550, rfoffset = 2550, lfoffset = 2550;
  // double lbDistance = 42731, rbDistance = 42731, rfDistance = 42731, lfDistance = 42731, masterDistance = 42731;
  double lboffset = 1, rboffset = 1, rfoffset = 1, lfoffset = 1;
  double lbDistance = 1, rbDistance = 1, rfDistance = 1, lfDistance = 1, masterDistance = 1;

  double _left_back_motor_value, _left_front_motor_value, _right_back_motor_value, _right_front_motor_value;

  double _motor_value_average;
  double _master_setpoint;  
  double _previous_setpoint;
  double _master_error_average;

  void Module_Move();

};
#endif
