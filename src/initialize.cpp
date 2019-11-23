#include "main.h"
#include "ports.h"
#include "opcontrol.h"

#include "utils/pid.h"
#include "utils/motor_controller.hpp"
#include "utils/robot/drive/mech_drive.hpp"
#include "initialize.h"

#include <list>
#include <map>
#include "globals.hpp"
#include "auton_control.hpp"
//Initialize Variables

//  Global Variables
pros::Controller master(CONTROLLER_MASTER);
AutonControl* auton_control = AutonControl::instance();

// Drive Variables:
std::array<double,3> left_back_pid_values = {0.66, 0, 0};
std::array<double,3> right_back_pid_values = {0.66, 0, 0};
std::array<double,3> left_front_pid_values = {0.66, 0, 0};
std::array<double,3> right_front_pid_values = {0.66, 0, 0};

std::array<double, 3> master_drive_pid_values = {0, 0.001, 0};

pros::Motor left_front_motor(LEFT_FRONT_MOTOR_PORT, false);
pros::Motor left_back_motor(LEFT_BACK_MOTOR_PORT, false);
pros::Motor right_front_motor(RIGHT_FRONT_MOTOR_PORT, true);
pros::Motor right_back_motor(RIGHT_BACK_MOTOR_PORT, true);

// Lift Variables:

std::array<double,3>  lift_pid_values = {0.618, 0, 1.454};
std::array<double, 3> master_lift_pid_values = {1, 0.001, 0};

pros::Motor left_lift_motor(LEFT_LIFT_MOTOR_PORT, LEFT_LIFT_MOTOR_ORIENTATION);
pros::Motor right_lift_motor(RIGHT_LIFT_MOTOR_PORT, RIGHT_LIFT_MOTOR_ORIENTATION);

double cube_height = 280;
double lift_min_height = 0;
double lift_max_height = 1800;
double lift_speed = 10;

// Angler Variables:

pros::Motor angler_motor(ANGLER_MOTOR_PORT, true);
std::array<double,3> angler_pid_values = {1, 0.1, 0};

double angler_speed = 16;
double angler_min_height = 0;
double angler_max_height = 2250;
Angler*  angler = new Angler();

// arm variables
pros::Motor arm_motor(4, false);
std::array<double,3> arm_pid_values = {1, 0, 0};
double arm_speed = 16;
double arm_min_height = 0;
double arm_max_height = 10000;
bool _is_moving = false;
bool _moving_up= false;
bool _manual_arm = false;
Arm* arm = new Arm();

pros::Motor intakeMotorLeft (2, true);
pros::Motor intakeMotorRight (3, false);

std::array<double, 3> pid_intake_left_values = {0.1,0,0};
std::array<double, 3> pid_intake_right_values = {0.1, 0, 0};
std::array<double, 3> master_intake_pid_values = {0.1,0.001,0};




void initialize()
{
  robot->create();
  angler->create();
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
