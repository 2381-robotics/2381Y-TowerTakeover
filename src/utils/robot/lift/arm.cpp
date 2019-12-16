#include "utils/arm.hpp"
#include "utils/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>

#include "utils/angler.hpp"

using namespace std;


// double Mech_Drive::Get_Speed() {
//   return (_left_front_motor_controller->Get_Speed()+_left_back_motor_controller->Get_Speed()+ _right_back_motor_controller->Get_Speed() + _right_front_motor_controller->Get_Speed())/4;
// }

void Arm::create()
{
  // Assign motors.
  _arm_motor = &arm_motor; //asdadasd
  // Initialize Pids.
  _arm_pid = new Pid(&arm_pid_values[0], &arm_pid_values[1], &arm_pid_values[2]);
  // Assign Variables
  _min_height = arm_min_height;
  _max_height = arm_max_height;
  _arm_speed = arm_speed;
  _target_height = 0;
  _is_moving = false;
  _moving_up = false;
  _manual_arm = false;
}

void Arm::Set_Target(double target_height)
{
    _target_height = target_height;

  if (target_height >= _max_height)
  {
    _target_height = _max_height;
  }
  else if (target_height <= _min_height)
  {
    _target_height = _min_height;
  }
}
void Arm::Move_Arm()
{
  _current_arm_height = (_arm_motor->get_position());
  _arm_power = _arm_pid->Update(_target_height, _current_arm_height);


//allows for clearance  
if(!_manual_arm) {
  // max height 2300
  // max height
        angler->Set_Target(0.40 * _current_arm_height);  
}
  _arm_motor->move(_arm_power);
 

  // pros::lcd::set_text(2, "angler power" + to_string(_angler_power));
//   pros::lcd::set_text(3, "angler position" + to_string(_angler_motor->get_position()));
}
Arm::Arm(){
  create();
}

void Arm::Increment_Arm(int increment)
{
  Set_Target(_target_height + increment * _arm_speed);
      //  pros::lcd::set_text(6, "current arm height" + to_string(_current_arm_height));

    // for clearances
    if (increment == 1) {
        _is_moving = true;
        _moving_up = true;
    }
    else if (increment == -1){
        _is_moving = true;
        _moving_up = false;
    }
    else {
        _is_moving = false;
        _moving_up = false;
    }
}
double Arm::Get_Height()
{
  return this->_current_arm_height;
}
double Arm::Get_Target()
{
  return this->_target_height;
}

