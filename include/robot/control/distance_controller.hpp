#ifndef DISTANCE_CONTROLLER_HPP_
#define DISTANCE_CONTROLLER_HPP_

#include "api.h"
#include "robot/control/pid.hpp"
#include "robot/control/motor_controller.hpp"

class Distance_Controller : public Motor_Controller {
 public:


  /**
   * Constructor
   * Accepts the kp, ki, and kd to determine the correction value
   * Defaults to (0,0,0)
   */
   Distance_Controller (double* kp = 0, double* ki = 0, double* kd = 0, pros::Motor* motor = 0);
  /**
   * Resets the error counts. It should be called when the PID loop is not
   * active to prevent integral windup.
   */

   double Set_Distance(double targetValue);
  /**
   * Returns the output of the PID controller correcting the input.
   * @param goal The current setpoint value
   * @param current_value The current value that will be compared with the setpoint
   */
  private: 
};

#endif
