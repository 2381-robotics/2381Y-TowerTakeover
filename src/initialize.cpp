#include "main.h"
#include "ports.h"
#include "opcontrol.h"

#include "main.h"
#include "ports.h"

#include "utils/pid.h"
#include "utils/motor_controller.h"
#include "utils/robot/drive/mech_drive.h"
#include "initialize.h"
#include "auto_drive.h"


#include <list>
#include <map>


void initialize() {

  pros::lcd::initialize();
  double kp = 1;

  // double kp = 0.618;
  double ki = 0.0;
  double kd = 0;
  // double kd = 0;
  lift_pid = new Pid(&kp, &ki, &kd);


  double left_back_pid_values [3] = {0.66, 0, 0}; //ku = 1.1
  double right_back_pid_values [3] = {0.6, 0, 0}; //ku = 1
  double left_front_pid_values [3] = {0.66, 0, 0};
  double right_front_pid_values [3] = {0.66, 0, 0}; //ku = 1.1

  pros::Motor left_front_motor (LEFT_FRONT_MOTOR_PORT, false);
  pros::Motor left_back_motor (LEFT_BACK_MOTOR_PORT, false);
  pros::Motor right_front_motor (RIGHT_FRONT_MOTOR_PORT, true);
  pros::Motor right_back_motor (RIGHT_BACK_MOTOR_PORT, true);
  double master_pid_values [3] = {0,0.001,0}; //ku = 1.10

  Motor_Controller* left_front_motor_controller = new Motor_Controller(&left_front_pid_values[0], &left_front_pid_values[1], &left_front_pid_values[2], &left_front_motor);
  Motor_Controller* left_back_motor_controller = new Motor_Controller(&left_back_pid_values[0], &left_back_pid_values[1], &left_back_pid_values[2], &left_back_motor);
  Motor_Controller* right_front_motor_controller = new Motor_Controller(&right_front_pid_values[0], &right_front_pid_values[1], &right_front_pid_values[2], &right_front_motor);
  Motor_Controller* right_back_motor_controller = new Motor_Controller(&right_back_pid_values[0], &right_back_pid_values[1], &right_back_pid_values[2], &right_back_motor);

  Mech_Drive* mech_drive = new Mech_Drive(left_front_motor_controller, left_back_motor_controller, right_front_motor_controller, right_back_motor_controller, &master_pid_values);
  Auto_Drive* auto_drive = new Auto_Drive(mech_drive);


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
