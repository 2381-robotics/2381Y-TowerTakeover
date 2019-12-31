#include "angler.hpp"
#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>

using namespace std;


void Angler::create()
{
  // Assign motors.
  _angler_motor = &angler_motor;
  // Initialize Pids.
  _angler_pid = new Pid(&angler_pid_values[0], &angler_pid_values[1], &angler_pid_values[2]);
  // Assign Variables
  _min_height = angler_min_height;
  _max_height = angler_max_height;
  _angler_speed = angler_speed;
  _target_height = 0;
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
void Angler::Move_Angler()
{
  if(STOP) {
    _angler_motor->move(0);
    return;
  }
 
  _angler_height = (_angler_motor->get_position());
  _angler_power = _angler_pid->Update(_target_height, _angler_height);
  // angler Coefficient is for keeping angler even / not tilted, not sure if it works.
  // Right now it's a constant and is additive, not sure if it should be multiplicative maybe
  _angler_motor->move(_angler_power);
  
  // pros::lcd::set_text(1, "Target height" + to_string(_target_height));
  // pros::lcd::set_text(2, "angler power" + to_string(_angler_power));
  // pros::lcd::set_text(3, "angler position" + to_string(_angler_motor->get_position()));
}
Angler::Angler(){
  create();
}

void Angler::task_fn(void* param) {
  while(true) {
    this->Move_Angler();
    pros::Task::delay_until(&now, DELAY_INTERVAL);
  }
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
void Angler::Smooth_Angler(int increment)
{ 
  Set_Target(_target_height + increment * _angler_speed);
}

double Angler::Get_Speed() {
  return _angler_motor->get_actual_velocity();
}



double Angler::Get_Height()
{
  return this->_angler_height;
}
double Angler::Get_Target()
{
  return this->_target_height;
}

void Angler::Reset(){
  
}
