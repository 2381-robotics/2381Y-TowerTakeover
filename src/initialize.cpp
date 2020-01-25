#include "main.h"
#include "ports.h"
#include "opcontrol.h"

#include "robot/control/pid.hpp"
#include "robot/control/motor_controller.hpp"
#include "robot/modules/drive/mech_drive.hpp"
#include "initialize.h"

#include <list>
#include <map>
#include "globals.hpp"
#include "autonomous/auton_control.hpp"
#include "master_controller.hpp"
#include "robot/task_factory.hpp"
#include "robot/modules/drive/position_tracker.hpp"
//Initialize Variables
using namespace pros;
using namespace std;
//  Global Variables
bool Competition_Env = false;
double autonomous_increment = 0;
Controller master(CONTROLLER_MASTER);
MasterController *master_control = MasterController::instance();
AutonControl *auton_control = AutonControl::instance();
bool STOP = false;

// Drive Variables:
array<double, 3> left_back_pid_values = {0.6, 0, 0};
array<double, 3> right_back_pid_values = {0.6, 0, 0};
array<double, 3> left_front_pid_values = {0.6, 0, 0};
array<double, 3> right_front_pid_values = {0.6, 0, 0};

array<double, 3> master_drive_pid_values = {0, 0.001, 0};

Motor left_front_motor(LEFT_FRONT_MOTOR_PORT, false);
Motor left_back_motor(LEFT_BACK_MOTOR_PORT, false);
Motor right_front_motor(RIGHT_FRONT_MOTOR_PORT, true);
Motor right_back_motor(RIGHT_BACK_MOTOR_PORT, true);
// Lift Variables:

array<double, 3> lift_pid_values = {0.618, 0, 1.454}; 
array<double, 3> master_lift_pid_values = {1, 0.001, 0}; 
 
Motor left_lift_motor(LEFT_LIFT_MOTOR_PORT, LEFT_LIFT_MOTOR_ORIENTATION); 
Motor right_lift_motor(RIGHT_LIFT_MOTOR_PORT, RIGHT_LIFT_MOTOR_ORIENTATION);

double cube_height = 280;
double lift_min_height = 0;
double lift_max_height = 1800;
double lift_speed = 10;

// Angler Variables:

Motor angler_motor(ANGLER_MOTOR_PORT, ANGLER_MOTOR_ORIENTATION);
array<double, 3> angler_pid_values = {2, 0.01, 0.05};

double angler_speed = 25;
double angler_min_height = 0;
double angler_max_height = 3600;
Angler *angler = new Angler();

// arm variables
pros::Motor arm_motor(20, true);
std::array<double,3> arm_pid_values = {1, 0, 0};
double arm_speed = 40;
double arm_min_height = 0;
double arm_max_height = 5000;
bool _is_moving = false;
bool _moving_up = false;
bool _manual_arm = false; 

Arm *arm = new Arm();

Motor intakeMotorLeft(LEFT_INTAKE_PORT, LEFT_INTAKE_ORIENTATION);
Motor intakeMotorRight(RIGHT_INTAKE_PORT, RIGHT_INTAKE_ORIENTATION);

array<double, 3> pid_intake_left_values = {0.6, 0, 0};
array<double, 3> pid_intake_right_values = {0.6, 0, 0};
array<double, 3> master_intake_pid_values = {0, 0.005, 0};

Intake *intake = new Intake();

//  Encoder Variables
array<int, 3> encoder_ports_left = {3,4,0}; //Top Port, Bottom Port, Inverted (0 or 1)
array<int, 3> encoder_ports_right = {1,2,0}; //Top Port, Bottom Port, Inverted (0 or 1)
array<int, 3> encoder_ports_back = {5,6,0}; //Top Port, Bottom Port, Inverted (0 or 1)

array<double, 3> wheel_diameters = {2.8, 2.8, 2.8}; // Wheel Diameters in Inches, (Left - Right - Back)
array<double, 3> wheel_offsets = {1.77, 1.77, 1.97};   //Perpindicular Wheel Offsets from Center in Inches, (Left - Right - Back)
Position_Tracker* position_tracker = Position_Tracker::instance();

ADIEncoder encoder_left(encoder_ports_left[0], encoder_ports_left[1], encoder_ports_left[2]);
ADIEncoder encoder_right(encoder_ports_right[0], encoder_ports_right[1], encoder_ports_right[2]);
ADIEncoder encoder_back(encoder_ports_back[0], encoder_ports_back[1], encoder_ports_back[2]);

void arm_task_fn(void *param)
{
  while (true)
  {
    arm->Run();
    pros::delay(DELAY_INTERVAL);
  }
}
void drive_task_fn(void *param)
{

  while (true)
  {
    // pros::lcd::set_text(0, to_string(pros::millis()));

    robot->drive->Run();
    pros::delay(DELAY_INTERVAL);
  }
}

void intake_task_fn(void* param) 
{
  while(true)
  {
    intake->Run();
    pros::delay(DELAY_INTERVAL);
  }
}

void angler_task_fn(void *param)
{
  while (true)
  {
    angler->Run();
    pros::delay(DELAY_INTERVAL);
  }
}
void tracking_task_fn(void *param)
{
  // pros::ADIEncoder encoder(encoder_ports_left[0], encoder_ports_left[1], false);

  while (true)
  {
    position_tracker->Track_Position();
    lcd::set_text(2,"left" + to_string((int)encoder_left.get_value()) + "right" + to_string((int)encoder_right.get_value() ));
    pros::delay(DELAY_INTERVAL);
  }
}


void initialize()
{
  lcd::initialize();
  robot->drive->Create();
  intake->Create();
  angler->Create();
  arm->Create();
  position_tracker->Create();
  std::string text("PROS");
  pros::Task angler_task(angler_task_fn, (void *)"PROS", TASK_PRIORITY_DEFAULT,
                         TASK_STACK_DEPTH_DEFAULT, "ANGLER_TASK");
  pros::Task arm_task(arm_task_fn, (void *)"PROS", TASK_PRIORITY_DEFAULT,
                         TASK_STACK_DEPTH_DEFAULT, "ARM_TASK");
  pros::Task drive_task(drive_task_fn, (void *)"PROS", TASK_PRIORITY_DEFAULT,
                         TASK_STACK_DEPTH_DEFAULT, "DRIVE_TASK");
  pros::Task intake_task(intake_task_fn, (void *)"PROS", TASK_PRIORITY_DEFAULT,
                        TASK_STACK_DEPTH_DEFAULT, "INTAKE_TASK");
  pros::Task tracking_task(tracking_task_fn, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "TRACKING_TASK");
  resetAuton1();
  // auton_control->define_auton(AutonControl::RedSmallSideAuton, auton1);
  // auton_control->define_auton(AutonControl::RedSmallSideAuton, auton1);

  auton_control->define_auton(AutonControl::Red5PointAuton, straightRedAuton);
  auton_control->define_auton(AutonControl::Blue5PointAuton, blue5PointAuton);
  auton_control->select_auton(AutonControl::Blue5PointAuton);
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
void competition_initialize()
{
  Competition_Env = true;
}
