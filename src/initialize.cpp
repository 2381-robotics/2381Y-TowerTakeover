#include "main.h"
#include "ports.h"
#include "opcontrol.h"

#include "utils/pid.h"
#include "utils/motor_controller.hpp"
#include "utils/robot/drive/mech_drive.hpp"
#include "initialize.h"
#include "auto_drive.h"

#include <list>
#include <map>
#include "globals.hpp"

std::array<double,3> left_back_pid_values;
std::array<double,3> right_back_pid_values;
std::array<double,3> left_front_pid_values;
std::array<double,3> right_front_pid_values;

pros::Motor left_front_motor (LEFT_FRONT_MOTOR_PORT, false);
pros::Motor left_back_motor (LEFT_BACK_MOTOR_PORT, false);
pros::Motor right_front_motor (RIGHT_FRONT_MOTOR_PORT, true);
pros::Motor right_back_motor (RIGHT_BACK_MOTOR_PORT, true);

std::array<double,3>  master_pid_values = {0,0.001,0}; //ku = 1.10

void initialize() {
  
  pros::lcd::initialize();

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
