#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "robot/control/pid.hpp"
#include "robot/control/distance_controller.hpp"
/**
 * Constructor taking in kp, ki, and kd
 */

Distance_Controller::Distance_Controller(double kp, double ki, double kd, pros::Motor* motor){
  this->motor = motor;
  pid = new Pid(kp, ki, kd);
}


double Distance_Controller::Set_Distance(double distance){
  double travelledDistance=  this->motor->get_position();
  double motorValue = this->pid->Update(distance, travelledDistance);
  (*motor).move_voltage(motorValue / 127 * 12000);
  return motorValue;
}

