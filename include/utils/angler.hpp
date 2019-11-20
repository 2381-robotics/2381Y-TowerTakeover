#ifndef ANGLER_HPP
#define ANGLER_HPP

#include "utils/robot/drive/drive.hpp"
#include "utils/motor_controller.hpp"
#include <array>

class Angler {
public:
  Angler();
  void Set_Angler(double angler);
  double Get_Speed();
  void create();

  double test;
private:
    Motor_Controller* _angler_motor_controller;

    Pid* _master_pid;
    double _angler_setpoint;

    double _angler_motor_value;
    double _motor_value_average;
    double _master_setpoint;
    double _previous_setpoint;
    double _master_error_average;


};
#endif
