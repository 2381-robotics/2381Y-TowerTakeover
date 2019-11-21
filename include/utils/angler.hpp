#ifndef ANGLER_HPP
#define ANGLER_HPP

#include "utils/robot/drive/drive.hpp"
#include "utils/motor_controller.hpp"
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
private:
    Motor_Controller* _angler_motor_controller;
    pros::Motor* _angler_motor;
    Pid* _angler_pid;
    Pid* _master_pid;
    double _angler_setpoint;

    double _angler_motor_value, _angler_height, _angler_power;
    double _min_height, _max_height, _angler_speed, _target_height;
    

};
#endif
