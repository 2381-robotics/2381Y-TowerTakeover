#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "utils/robot/robot.hpp"
#include <array>
#include "api.h"
#include "ports.h"
#include "okapi/api.hpp"

extern Robot* robot;

extern std::array<double,3> left_back_pid_values;
extern std::array<double,3> right_back_pid_values;
extern std::array<double,3> left_front_pid_values;
extern std::array<double,3> right_front_pid_values;

extern std::array<double, 3> master_pid_values; //ku = 1.10

extern std::array<double,3> lift_pid_values, master_lift_pid_values;

extern pros::Motor left_front_motor;
extern pros::Motor left_back_motor;
extern pros::Motor right_front_motor;
extern pros::Motor right_back_motor;

extern pros::Motor left_lift_motor;
extern pros::Motor right_lift_motor;

extern pros::Controller master;


#endif
