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
  void Smooth_Angler_Auton(int increment);
  void Toggle_Extension(int increment);
  double Get_Height();
  double Get_Target();
  double Get_Speed();
  std::string name = "Angler";
  void Stop() override;
  void Create() override;

  void Multiply_Max_Speed(double multiplier);

  void Auto_Angler(int increment, bool disable = false);
  void Reset() override;

  double _angler_motor_value, _angler_height, _angler_power,  _angler_speed, _auton_target_height, _target_height = 0;
  double _min_height, _max_height;

  void Override_Mode(int power);

protected:

    double _previous_target = 0;
    double _max_angler_speed = 0;
    double Get_Real_Target();
    double _auto_angler_increment = 0;



  bool isOverrideMode = false;
  float override_power = 0;
  void Move_Motor() override;
    Motor_Controller* _angler_motor_controller;
    pros::Motor* _angler_motor;
    Pid* _angler_pid;
    Pid* _master_pid;
    double _angler_setpoint;
};
#endif
