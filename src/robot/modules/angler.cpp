#include "angler.hpp"
#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>

using namespace std;

void Angler::Move_Motor() {
  _angler_height = (_angler_motor->get_position());
  double real_target = Get_Real_Target();
  if(isOverrideMode) {
    _angler_power = override_power;
    _angler_pid->Update(real_target, _angler_height);
    _angler_pid->Update(_max_height * (pow(real_target / _max_height, 1)), _angler_height);
  }
  else {
    _angler_power = _angler_pid->Update(_max_height* (pow(real_target/_max_height,0.9)), _angler_height);
  }

  // angler Coefficient is for keeping angler even / not tilted, not sure if it works.
  // Right now it's a constant and is additive, not sure if it should be multiplicative maybe
  _angler_motor->move(_angler_power);
}


double Angler::Get_Real_Target()
{
  double real_target = _target_height;
  if(_target_height - _previous_target > _max_angler_speed) {
     real_target = _previous_target + _max_angler_speed;
  }
  else if (_previous_target - _target_height > 2* _max_angler_speed)
  {
    real_target = _previous_target - 2* _max_angler_speed;
  }
  _previous_target = real_target;
  return real_target;
}
void Angler::Stop() {
  _angler_motor->move(0);
  return;
}
void Angler::Multiply_Max_Speed(double multiplier)
{
  _max_angler_speed = angler_speed* multiplier;
}

void Angler::Set_Target(double target_height)
{

  if (target_height >= _max_height)
  {
    _target_height = _max_height;
  }
  else if (target_height <= _min_height)
  {
    _target_height = _min_height;

  }
  else
  {
    _target_height = target_height;

  }

}
Angler::Angler(){
 
}
void Angler::Create() {
  // Assign motors.
  _angler_motor = &angler_motor;
  // Initialize Pids.
  _angler_pid = new Pid(&angler_pid_values[0], &angler_pid_values[1], &angler_pid_values[2]);
  // Assign Variables
  _min_height = angler_min_height;
  _max_height = angler_max_height;
  _angler_speed = angler_speed;
  _max_angler_speed =  2*angler_speed;
}

void Angler::Toggle_Extension(int increment)
{
  // If its not at max, set to max - if it is at max set to min - 
  if(increment > 0) {
    if (_angler_height <= _max_height - 100)
    {
      Set_Target(_max_height);
    }
    else
    {
      Set_Target(_min_height);
    };
  }
}

void Angler::Override_Mode(int toggle)
{
  // pros::lcd::set_text(4, "Override Mode" + to_string(isOverrideMode));

  if(toggle == 0 && isOverrideMode) 
  {
    
    _target_height = Get_Height();
    // _angler_pid->ResetError();
    override_power = 0;
    isOverrideMode = false;
  }
   else if (toggle == 1) {
    _target_height = Get_Height();
    // _angler_pid->ResetError();
    isOverrideMode = true;  
   } else if (toggle == -1) {
     _target_height = Get_Height();
   }
}

void Angler::Smooth_Angler(int increment)
{ 
  if( increment != 0)
  {
    _auto_angler_increment = 0;
  }

  if(isOverrideMode)
  {
    if(increment > 0)
    {
      override_power = 100*increment;
    } else if (increment < 0)
    {
      // override_power = -80;
      override_power = 0;
    } else 
    {
      override_power = 0;
    }
    return;

  }
  Set_Target(_target_height + increment * _angler_speed);
                // pros::lcd::set_text(5, "current angler position" + to_string(_target_height));

}

void Angler::Auto_Angler(int increment, bool disable)
{
  if (increment != 0)
  {
      _auto_angler_increment = increment;
  }
  if (disable)
  {
     _auto_angler_increment = 0;
  }
  Set_Target(_target_height + _auto_angler_increment * _angler_speed*1.2);
}


double Angler::Get_Speed() {
  return _angler_motor->get_actual_velocity();
}



double Angler::Get_Height()
{
  return _angler_motor->get_position();
}
double Angler::Get_Target()
{
  return this->_target_height;
}

void Angler::Reset(){
  
}


// for auton 
void Angler::Smooth_Angler_Auton(int increment)
{ 
  _auton_target_height = _target_height + increment * _angler_speed;

  if (_auton_target_height >= _max_height/2.5)
  {
    _target_height = _max_height/2.5;
  }
  else if (_auton_target_height <= _min_height)
  {
    _target_height = _min_height;

  }
  else
  {
    _target_height = _auton_target_height;
  }

}