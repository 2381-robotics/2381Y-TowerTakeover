#ifndef ARM_HPP
#define ARM_HPP

#include "utils/robot/drive/drive.hpp"
#include "utils/motor_controller.hpp"
#include <array>

class Arm {
public:
  Arm();
  void Set_Target(double target_height);
  void Move_Arm();
  void Increment_Arm(int increment);
  double Get_Height();
  double Get_Target();
  double Get_Speed();
  void create();

  double test;
private:
    Motor_Controller* _arm_motor_controller;
    pros::Motor* _arm_motor;
    Pid* _arm_pid;
    Pid* _master_pid;
    double _arm_setpoint;

    double _arm_motor_value, _current_arm_height, _arm_power;
    double _min_height, _max_height, _arm_speed, _target_height;
    bool _is_moving, _moving_up, _manual_arm;
  

};
#endif
