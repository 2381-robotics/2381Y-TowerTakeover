#include "arm.hpp"
#include "pros/misc.h"
#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>
#include "robot/modules/angler.hpp"

using namespace std;
using namespace pros;

void Arm::Set_Target(double target_height)
{
  if (Get_Height() < 200 && angler->Get_Height() < 200){
    arm_mode = 0;
  }
  else if (Get_Height() < 200 && angler->Get_Height() >= 200){
    arm_mode = 2;
  }
  else if (Get_Height() >= 200 && angler->Get_Height() < 200){
    arm_mode = 1;
  }

  if (arm_mode != 2) {
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
}

void Arm::Stop()
{
  _arm_motor->move(0);
}

void Arm::Move_Motor()
{
 
  _current_arm_height = (_arm_motor->get_position());
  double real_target = Get_Real_Target();

  _arm_power = _arm_pid->Update(real_target, _current_arm_height);
 
 
  if (real_target <= 1400 && real_target >= 600)
  {
    angler->Auto_Angler(0, true);
    angler->Set_Target(1.1 * (real_target - 600));
    angler->_min_height = 1.1 * (real_target - 600);
    pros::lcd::set_text(6, to_string(angler->_min_height));

  }
  //allows for clearance
  // pros::lcd::set_text(3, "arm " + to_string((int)_arm_motor->get_position()) + "ang" + to_string((int)angler->Get_Height()) + "power ang" + to_string((int)_arm_power));

  _arm_motor->move(_arm_power);
}


double Arm::Get_Real_Target()
{
  double real_target = _target_height;
  if(_target_height - _previous_target > _max_arm_speed) {
     real_target = _previous_target + _max_arm_speed;
  }
  else if (_previous_target - _target_height > 0.5* _max_arm_speed)
  {
    real_target = _previous_target - 0.5* _max_arm_speed;
  }
  _previous_target = real_target;
  
  return real_target;
}


Arm::Arm()
{

}


void Arm::Create() {
  // Assign motors.
  _arm_motor = &arm_motor; //asdadasd
  // Initialize Pids.
  _arm_pid = new Pid(arm_pid_values[0], arm_pid_values[1], arm_pid_values[2]);
  // Assign Variables
  _min_height = arm_min_height;
  _max_height = arm_max_height;
  _arm_speed = arm_speed;
  _max_arm_speed = 2 * arm_speed;
  _is_moving = false;
  _moving_up = false;
  _manual_arm = false;
}
void Arm::Arm_Macro(int increment)
{
  if(current_macro + increment >= 0 && current_macro + increment < arm_heights.size() && increment!=0)
  {
    Set_Target(arm_heights[increment]);
  }

}
void Arm::Reset()
{
}

void Arm::Increment_Arm(int increment)
{

  if(increment!=0)
  {
    Set_Target(_target_height + increment * _arm_speed);
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

// double Mech_Drive::Get_Speed() {
//   return (_left_front_motor_controller->Get_Speed()+_left_back_motor_controller->Get_Speed()+ _right_back_motor_controller->Get_Speed() + _right_front_motor_controller->Get_Speed())/4;
// }
