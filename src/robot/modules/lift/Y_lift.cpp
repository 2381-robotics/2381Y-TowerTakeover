#include "Y_lift.hpp"
#include "legacy_globals.hpp"
#include "robot/control/motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>

Y_Lift::Y_Lift(){

};

void Y_Lift::Create() {
    // Assign motors.
    // _left_lift_motor = &left_lift_motor;
    // _right_lift_motor = &right_lift_motor;
    // Initialize Pids.
    _lift_pid = new Pid(lift_pid_values[0], lift_pid_values[1], lift_pid_values[2]);
    _master_lift_pid = new Pid(master_lift_pid_values[0], master_lift_pid_values[1], master_lift_pid_values[2]);
    // Assign Variables
    _cube_height = cube_height;
    _min_height = lift_min_height;
    _max_height = lift_max_height;
    _lift_speed = lift_speed;
    _target_height = 0;
}
void Y_Lift::Set_Target(double target_height) {
    if(target_height > _max_height) {
        _target_height = _max_height;
        
    } else if (target_height < _min_height) {
        _target_height = _min_height;
    } else {
        _target_height = target_height;
    }
}
void Y_Lift::Move_Lift() {
    _lift_height = (_left_lift_motor->get_position() + _right_lift_motor->get_position()) / 2;
    _lift_difference = (_left_lift_motor->get_position() - _right_lift_motor->get_position());
    _lift_power = _lift_pid->Update(_target_height, _lift_height);
    // Lift Coefficient is for keeping lift even / not tilted, not sure if it works.
    // Right now it's a constant and is additive, not sure if it should be multiplicative maybe?
    _lift_coefficient = _master_lift_pid->Update(0, _lift_difference);
    

    _right_lift_motor->move(_lift_power);
    _left_lift_motor->move(_lift_power);
}
void Y_Lift::Increment_Height(int increment) {
    Set_Target(_target_height + increment*_cube_height);
}
void Y_Lift::Smooth_Lift(int increment) {
    Set_Target(_target_height + increment*_lift_speed);
}
double Y_Lift::Get_Height() {
    return this->_lift_height;
}
double Y_Lift::Get_Target() {
    return this->_target_height;
}
