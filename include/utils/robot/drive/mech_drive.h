#ifndef MECH_DRIVE_H
#define MECH_DRIVE_H


#include "utils/robot/drive/drive.h"
#include "utils/motor_controller.h"
#include <array>

class Mech_Drive : public Drive {
public:
  Mech_Drive(Motor_Controller* left_front_motor_controller = 0, Motor_Controller* left_back_motor_controller = 0, Motor_Controller* right_front_motor_controller = 0, Motor_Controller* right_back_motor_controller = 0, double(*master_pid_values)[3]  = 0);
  void Set_Drive(double left_x, double left_y, double right_x, double right_y) override;
  double Get_Speed() override;

  double test;
private:
    Motor_Controller* _left_front_motor_controller;
    Motor_Controller* _left_back_motor_controller;
    Motor_Controller* _right_front_motor_controller;
    Motor_Controller* _right_back_motor_controller;
    Pid* _master_pid;
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
    double _master_error_average;


};
#endif
