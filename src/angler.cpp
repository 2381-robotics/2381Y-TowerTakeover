#include "utils/angler.hpp"
#include "utils/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>

using namespace std;


// double Mech_Drive::Get_Speed() {
//   return (_left_front_motor_controller->Get_Speed()+_left_back_motor_controller->Get_Speed()+ _right_back_motor_controller->Get_Speed() + _right_front_motor_controller->Get_Speed())/4;
// }
void Angler::Set_Angler(double angler){
  _previous_setpoint = _master_setpoint;
  _angler_setpoint = (100);
  pros::lcd::set_text(5, "old master setpoint:" + std::to_string((_previous_setpoint)));
  _master_setpoint = _angler_setpoint;
  pros::lcd::set_text(6, "new master setpoint:" + std::to_string((_master_setpoint)));
  
  double tuning_coefficient = _master_pid->Update(0, _master_error_average);
  pros::lcd::set_text(2, "tuning coeff:" + std::to_string(tuning_coefficient));

 
  
  _angler_motor_value = _angler_motor_controller->Set_Speed(_angler_setpoint * tuning_coefficient);
  _motor_value_average = _angler_motor_value;

  pros::lcd::set_text(3, "motor value average:" + std::to_string((_motor_value_average)));
  if(_master_setpoint >= 0){
    _master_error_average = _motor_value_average - _master_setpoint;
  } else{
    _master_error_average = _master_setpoint - _motor_value_average;
  }

  pros::lcd::set_text(4, "master error average:" + std::to_string((_master_error_average)));

}
//Empty default constructor for blank factory arguments.
Angler::Angler(){}
void Angler::create() {

    _angler_motor_controller = new Motor_Controller(&angler_pid_values[0], &angler_pid_values[1], &angler_pid_values[2], &_angler_motor);
    _master_pid = new Pid(&(master_drive_pid_values)[0], &(master_drive_pid_values)[1], &(master_drive_pid_values)[2]);
    _master_error_average = 0;
    _master_setpoint = 1;
}
