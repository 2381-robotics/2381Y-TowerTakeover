#include "utils/motor_controller.h"
#include "api.h"
#include "utils/pid.h"
/**
 * Constructor taking in kp, ki, and kd
 */

Motor_Controller::Motor_Controller(double* kp, double* ki, double* kd, pros::Motor* motor){
  this->motor = motor;
  pid = new Pid(kp, ki, kd);
}

void Motor_Controller::Set_Speed(double targetValue){
  double motorSpeed = this->motor->get_actual_velocity();
  double motorValue = this->pid->Update(targetValue, motorSpeed);
  (*motor).move(motorValue);
}
