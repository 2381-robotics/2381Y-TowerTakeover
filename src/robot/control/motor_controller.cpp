#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "robot/control/pid.hpp"
/**
 * Constructor taking in kp, ki, and kd
 */

Motor_Controller::Motor_Controller(double* kp, double* ki, double* kd, pros::Motor* motor){
  this->motor = motor;
  pid = new Pid(kp, ki, kd);
}


double Motor_Controller::Get_Speed () {
  return this->motor->get_actual_velocity();
}

double Motor_Controller::Get_Distance (){
  return this->motor->get_position();
}

double Motor_Controller::Set_Speed(double targetValue){
  double motorSpeed = this->motor->get_actual_velocity();
  double motorValue = this->pid->Update(targetValue, motorSpeed);
  motor->move_voltage(motorValue/127*12000);
  return motorValue;
}

