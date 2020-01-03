#ifndef ANGLER_HPP
#define ANGLER_HPP

#include "robot/module.hpp"
#include "robot/modules/drive/drive.hpp"
#include "robot/control/motor_controller.hpp"
#include <array>

class Angler : public Module{
public:
  Angler();
  void Set_Target(double target_height);
  void Smooth_Angler(int increment);
  void Toggle_Extension(int increment);
  double Get_Height();
  double Get_Target();
  double Get_Speed();
  std::string name = "Angler";
  void Stop() override;
  void Create() override;

  void Reset() override;
  double _angler_motor_value, _angler_height, _angler_power, _target_height, _angler_speed;
  double _min_height, _max_height;

private:
  void Move_Motor() override;
    Motor_Controller* _angler_motor_controller;
    pros::Motor* _angler_motor;
    Pid* _angler_pid;
    Pid* _master_pid;
    double _angler_setpoint;
};
#endif
