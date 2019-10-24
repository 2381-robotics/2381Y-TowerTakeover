#include "utils/robot/drive/mech_drive.hpp"
#include "../../motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>


double Mech_Drive::Get_Speed() {
  return (_left_front_motor_controller->Get_Speed()+_left_back_motor_controller->Get_Speed()+ _right_back_motor_controller->Get_Speed() + _right_front_motor_controller->Get_Speed())/4;
}
void Mech_Drive::Set_Drive(double left_x, double left_y, double right_x, double right_y){

//setpoint is the desired motor value for each motor such that the ratio 
//between pairs of motors (left and right sides for normal driving, diagonals for sliding)
//is equal to the direction of travel we want to travel in 

//each setpoint is calculated using contrller inputs to determine the power required in each motor to 
//achieve a desired direction (Eg. More power to left side if want to turn right)
  _left_back_setpoint = (left_y - left_x + std::abs(right_x)*(right_x)/127);
  _left_front_setpoint = (left_y + left_x + std::abs(right_x)*(right_x)/127);
  _right_back_setpoint = (left_y + left_x - std::abs(right_x)*(right_x)/127);
  _right_front_setpoint = (left_y - left_x  - std::abs(right_x)*(right_x)/127);
  pros::lcd::set_text(0, "old master setpoint:" + std::to_string((_master_setpoint)));
  //take the average setpoint to be used in the tuning coefficent PID

  //taking average because its a general variable that affects the magnitude of the vector and not direction
  _master_setpoint = (_left_back_setpoint + _left_front_setpoint + _right_back_setpoint + _right_front_setpoint)/4;
  pros::lcd::set_text(1, "new master setpoint:" + std::to_string((_master_setpoint)));

//found during testing that the 1:1 ratio between pid valud to motor was incorrect 

//currently using another pid, _master_pid, to bridge the gap between the actual intended speed
//and the actual speed in rpm

//NOTE: the tuning coeefficent is a number that increases the magnitude of a directino vector and 
//      does not impact the direction of the vector
  double tuning_coefficient = _master_pid->Update(0, _master_error_average);
  pros::lcd::set_text(2, "tuning coeff:" + std::to_string(tuning_coefficient));

  // pros::lcd::set_text(0+4, "left_back_motor:" + std::to_string((_left_back_setpoint)));
  //
  // pros::lcd::set_text(2+4, "left_front_motor:" + std::to_string((_left_front_setpoint)));
  //
  // pros::lcd::set_text(1+4, "right_back_motor:" + std::to_string((_right_back_setpoint)));
  //
  // pros::lcd::set_text(3+4, "right_front_motor:" + std::to_string((_right_front_setpoint)));


  // pros::lcd::set_text(0, "left_back_motor:" + std::to_string(_left_back_motor_controller->Set_Speed(_left_back_setpoint)));
  //
  // pros::lcd::set_text(2, "left_front_motor:" + std::to_string( _left_front_motor_controller->Set_Speed(_left_front_setpoint)));
  //
  // pros::lcd::set_text(1, "right_back_motor:" + std::to_string(_right_back_motor_controller->Set_Speed(_right_back_setpoint)));
  //
  // pros::lcd::set_text(3, "right_front_motor:" + std::to_string(  _right_front_motor_controller->Set_Speed(_right_front_setpoint)));

  //here using method Set_Speed, the target values for motor are run through the PID in motor_controller.cpp
  //It creates motor values that are then put into the robots drive code and it moves
  //the setpoint values are constantly being updated as the user moves the remote so the motor value is also being updated
  //NOTE: tunning_coefficent starts at zero which is why it is used to define itself later in the code
  _left_back_motor_value = _left_back_motor_controller->Set_Speed(_left_back_setpoint * tuning_coefficient);
  _left_front_motor_value = _left_front_motor_controller->Set_Speed(_left_front_setpoint * tuning_coefficient);
  _right_back_motor_value = _right_back_motor_controller->Set_Speed(_right_back_setpoint * tuning_coefficient);
  _right_front_motor_value = _right_front_motor_controller->Set_Speed(_right_front_setpoint * tuning_coefficient);
  _motor_value_average = (_left_back_motor_value + _left_front_motor_value +  _right_back_motor_value +  _right_front_motor_value)/4;
  pros::lcd::set_text(3, "motor value average:" + std::to_string((_motor_value_average)));
  if(_master_setpoint >= 0){
    // the setpoint is the controller input and is the target that the driver wants to achieve
    //the motor value is the actual motor value that is measured from the motor 
    //master error is used in the higher level PID to see how far the real speed and intended speed is
    _master_error_average = _motor_value_average - _master_setpoint;
  } else{
    _master_error_average = _master_setpoint - _motor_value_average;
  }

  pros::lcd::set_text(4, "master error average:" + std::to_string((_master_error_average)));

}
//Empty default constructor for blank factory arguments.
Mech_Drive::Mech_Drive(){}
//creates new objects which run the first level PID in motor_controller.cpp
void Mech_Drive::create() {
    _left_front_motor_controller = new Motor_Controller(&left_front_pid_values[0], &left_front_pid_values[1], &left_front_pid_values[2], &left_front_motor);
    _left_back_motor_controller = new Motor_Controller(&left_back_pid_values[0], &left_back_pid_values[1], &left_back_pid_values[2], &left_back_motor);
    _right_front_motor_controller = new Motor_Controller(&right_front_pid_values[0], &right_front_pid_values[1], &right_front_pid_values[2], &right_front_motor);
    _right_back_motor_controller = new Motor_Controller(&left_front_pid_values[0], &left_front_pid_values[1], &left_front_pid_values[2], &left_front_motor);
    _master_pid = new Pid(&(master_pid_values)[0], &(master_pid_values)[1], &(master_pid_values)[2]);
    //here the HIGHER LEVEL PID stuff is defined so the tunning coefficent loop has a base value to go off of
    _master_error_average = 0;
    _master_setpoint = 1;
}
