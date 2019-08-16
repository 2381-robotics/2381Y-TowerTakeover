#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "api.h"
#include "pid.h"

class Motor_Controller {
 public:
  /**
   * Constructor
   * Accepts the kp, ki, and kd to determine the correction value
   * Defaults to (0,0,0)
   */
   Motor_Controller(double* kp = 0, double* ki = 0, double* kd = 0, pros::Motor* motor = 0);
  /**
   * Resets the error counts. It should be called when the PID loop is not
   * active to prevent integral windup.
   */
  double Set_Speed(double targetValue);
  Pid* pid;
  pros::Motor* motor;
  /**
   * Returns the output of the PID controller correcting the input.
   * @param goal The current setpoint value
   * @param current_value The current value that will be compared with the setpoint
   */
  double Update(double setpoint, double current_value);

};

#endif
