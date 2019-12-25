#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "robot/robot.hpp"
#include <array>
#include "api.h"
#include "ports.h"
#include "okapi/api.hpp"
#include "autonomous/auton_control.hpp"
#include "robot/modules/angler.hpp"
#include "robot/modules/arm.hpp"
#include "robot/modules/intake.hpp"
#include "master_controller.hpp"
#include "autonomous/auto_sequence.hpp"

#define DELAY_INTERVAL 20

using namespace std;


extern Robot* robot;
extern AutonControl* auton_control;
extern pros::Controller master;
extern MasterController* master_control;

// Autonomous
extern AutoSequence* auton1;

extern void resetAuton1();

extern double autonomous_increment;

extern void driver();

extern bool Competition_Env;

// Drive
extern std::array<double,3> left_back_pid_values, right_back_pid_values, left_front_pid_values, right_front_pid_values;
extern std::array<double, 3> master_drive_pid_values; //ku = 1.10

extern pros::Motor left_front_motor, left_back_motor, right_front_motor, right_back_motor;

//Angler
// extern angler;
extern std::array<double,3> angler_pid_values;
extern pros::Motor angler_motor;
extern double angler_min_height, angler_max_height, angler_speed;
extern Angler* angler;
//Arm
extern std::array<double,3> arm_pid_values;
extern pros::Motor arm_motor;
extern double arm_min_height, arm_max_height, arm_speed;
extern Arm* arm;
// Lift
extern std::array<double,3> lift_pid_values, master_lift_pid_values;
extern pros::Motor left_lift_motor, right_lift_motor;
extern double cube_height, lift_max_height, lift_min_height, lift_speed;

//intake
extern std::array<double, 3> pid_intake_left_values, pid_intake_right_values;
extern std::array<double, 3> master_intake_pid_values;
extern pros::Motor intakeMotorLeft, intakeMotorRight;
extern Intake* intake;


#endif
