#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "utils/robot/robot.hpp"
#include <array>
#include "api.h"
#include "ports.h"
#include "okapi/api.hpp"

#define MTR_PORT_LFRONT 1
#define MTR_PORT_RFRONT 2
#define MTR_PORT_LBACK 3
#define MTR_PORT_RBACK 4
#define MTR_PORT_UTIL1 5
#define MTR_PORT_UTIL2 6
#define MTR_PORT_UTIL3 7
#define MTR_PORT_UTIL4 8


extern Robot* robot;

extern std::array<double,3> left_back_pid_values;
extern std::array<double,3> right_back_pid_values;
extern std::array<double,3> left_front_pid_values;
extern std::array<double,3> right_front_pid_values;

extern pros::Motor left_front_motor;
extern pros::Motor left_back_motor;
extern pros::Motor right_front_motor;
extern pros::Motor right_back_motor;

extern std::array<double,3> master_pid_values; //ku = 1.10

#endif
