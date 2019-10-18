#include "utils/motor_controller.hpp"
#include "utils/robot/drive/mech_drive.hpp"

#include "api.h"
#include "auto_drive.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "utils/pid.h"


/**
 * Constructor taking in kp, ki, and kd
 */

Auto_Drive::Auto_Drive(Drive* drive){
  this->drive = drive;
}

std::tuple<double, double> Auto_Drive::Convert(double speed, double direction){
  std::tuple<double, double> drive_coordinates;
  double x = sin(direction* (180/M_PI)) *speed;
  double y = cos(direction* (180/M_PI)) *speed;
  drive_coordinates = std::make_tuple(x, y);
  return drive_coordinates;
}

volatile void Auto_Drive::Set_Point_Drive(double *speed, double *direction, double *distance){
  _speed = speed;
  _direction = direction;
    std::tuple<double, double> drive_convert = this->Convert(*_speed, *_direction);
  this->drive->Set_Drive(0, std::get<0>(drive_convert), std::get<1>(drive_convert), 0);

}




//___int64_t_definedvoid Drive(double left_x, double left_y, double right_x, double right_y);
