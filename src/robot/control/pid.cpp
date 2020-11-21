#include "robot/control/pid.hpp"
#include "api.h"

/**
 * Constructor taking in kp, ki, and kd
 */
Pid::Pid(double kp, double ki, double kd) {
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
  ResetError();
}

/**
 * Resets error to prevent integral windup
 */
void Pid::ResetError() {
  error_sum_ = 0.0;
  last_error_ = 0.0;
}

/**
 * Every update period, this is called to calculate a new PID output
 */

double Pid::Update(double setpoint, double current_value) {
  double error = setpoint - current_value;
  double p = kp_ * error;
  error_sum_ += error;
  


  double i = ki_ * error_sum_;
  double d_error = error - last_error_;
  double d = kd_ * d_error;
  last_error_ = error;
  return p + i + d;
}

void Pid::Set_Error(double Error)
{
  error_sum_ = Error;
}