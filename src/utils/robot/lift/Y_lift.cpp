#include "utils/robot/lift/Y_lift.hpp"
#include "../../motor_controller.hpp"
#include "api.h"
#include "globals.hpp"
#include <array>

Y_Lift::Y_Lift(){};

void Y_Lift::create() {
    _left_lift_motor = &left_lift_motor;
    _right_lift_motor = &right_lift_motor;
    _lift_pid = new Pid(&lift_pid_values[0], &lift_pid_values[1], &lift_pid_values[2]);
    _master_lift_pid = new Pid(&master_lift_pid_values[0], &master_lift_pid_values[1], &master_lift_pid_values[2]);
}
void Y_Lift::Set_Height(double target_height) {
    _target_height = target_height;
    _lift_height = (_left_lift_motor->get_position() + _right_lift_motor->get_position()) / 2;
    _lift_difference = (_left_lift_motor->get_position() - _right_lift_motor->get_position());
    _lift_power = _lift_pid->Update(_target_height, _lift_height);
    _lift_coefficient = _master_lift_pid->Update(0, _lift_difference);
    _right_lift_motor->move(_lift_power - _lift_coefficient);
    _left_lift_motor->move(_lift_power + _lift_coefficient);
}