#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "utils/robot/robot.hpp"
#include <array>
#include "api.h"
#include "ports.h"
#include "okapi/api.hpp"
//  Global Variable Declarations
extern Robot* robot;
extern pros::Controller master;

// Drive
extern std::array<double,3> left_back_pid_values, right_back_pid_values, left_front_pid_values, right_front_pid_values;
extern std::array<double, 3> master_drive_pid_values; //ku = 1.10
extern pros::Motor left_front_motor, left_back_motor, right_front_motor, right_back_motor;

// Lift
extern std::array<double,3> lift_pid_values, master_lift_pid_values;
extern pros::Motor left_lift_motor, right_lift_motor;
extern double cube_height, lift_max_height, lift_min_height, lift_speed;

#endif
