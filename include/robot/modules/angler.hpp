#ifndef ANGLER_HPP
#define ANGLER_HPP

#include "robot/modules/drive/drive.hpp"
#include "robot/control/motor_controller.hpp"
#include <array>

class Angler {
public:
  Angler();
  void Set_Target(double target_height);
  void Move_Angler();
  void Smooth_Angler(int increment);
  void Toggle_Extension(int increment);
  double Get_Height();
  double Get_Target();
  double Get_Speed();
  void create();

  double test;
      double _angler_motor_value, _angler_height, _angler_power, _target_height, _angler_speed;

private:
    Motor_Controller* _angler_motor_controller;
    pros::Motor* _angler_motor;
    Pid* _angler_pid;
    Pid* _master_pid;
    double _angler_setpoint;

    double _min_height, _max_height;
    

};
#endif
