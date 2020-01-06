#ifndef ARM_HPP
#define ARM_HPP

#include "robot/module.hpp"
#include "robot/modules/drive/drive.hpp"
#include "robot/control/motor_controller.hpp"
#include <array>

class Arm : public Module {
public:
  Arm();
  void Set_Target(double target_height);
  void Increment_Arm(int increment);
  double Get_Height();
  double Get_Target();
  double Get_Speed();
  std::string name = "Arm";

  void Create() override;
  void Stop() override;
  void Reset() override;

protected:

    void Move_Motor() override;
  double _arm_power, _current_arm_height;
    Motor_Controller* _arm_motor_controller;
    pros::Motor* _arm_motor;
    Pid* _arm_pid;
    double _arm_setpoint;

    double _arm_motor_value;
    double _min_height, _max_height, _arm_speed, _target_height;
    bool _is_moving, _moving_up, _manual_arm;
  

};
#endif
