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

Motor angler_motor(ANGLER_MOTOR_PORT, true);
array<double, 3> angler_pid_values = {1, 0.1, 0};

double angler_speed = 24;
double angler_min_height = 0;
double angler_max_height = 3400;
Angler *angler = new Angler();

// arm variables
Motor arm_motor(1, false);
array<double, 3> arm_pid_values = {1, 0, 0};
double arm_speed = 16;
double arm_min_height = 0;
double arm_max_height = 10000;
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

lv_obj_t *myButton;
lv_obj_t *myButtonLabel;
lv_obj_t *myLabel;

lv_style_t myButtonStyleREL; //relesed style
lv_style_t myButtonStylePR;  //pressed style

static lv_res_t btn_click_action(lv_obj_t *btn)
{
  uint8_t id = lv_obj_get_free_num(btn); //id usefull when there are multiple buttons

  if (id == 0)
  {
    char buffer[100];
    sprintf(buffer, "button was clicked %i milliseconds from start", pros::millis());
    lv_label_set_text(myLabel, buffer);
  }

  return LV_RES_OK;
}

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


void initialize()
{
  lcd::initialize();
  robot->drive->Create();
  intake->Create();
  angler->Create();
  arm->Create();


  std::string text("PROS");
  pros::Task angler_task(angler_task_fn, (void *)"PROS", TASK_PRIORITY_DEFAULT,
                         TASK_STACK_DEPTH_DEFAULT, "ANGLER_TASK");
  pros::Task arm_task(arm_task_fn, (void *)"PROS", TASK_PRIORITY_DEFAULT,
                         TASK_STACK_DEPTH_DEFAULT, "ARM_TASK");
  pros::Task drive_task(drive_task_fn, (void *)"PROS", TASK_PRIORITY_DEFAULT,
                         TASK_STACK_DEPTH_DEFAULT, "DRIVE_TASK");
  pros::Task intake_task(intake_task_fn, (void *)"PROS", TASK_PRIORITY_DEFAULT,
                        TASK_STACK_DEPTH_DEFAULT, "INTAKE_TASK");

  // robot->module_list = {{1, angler}};

  // lv_style_copy(&myButtonStyleREL, &lv_style_plain);
  // myButtonStyleREL.body.main_color = LV_COLOR_MAKE(150, 0, 0);
  // myButtonStyleREL.body.grad_color = LV_COLOR_MAKE(0, 0, 150);
  // myButtonStyleREL.body.radius = 0;
  // myButtonStyleREL.text.color = LV_COLOR_MAKE(255, 255, 255);

  // lv_style_copy(&myButtonStylePR, &lv_style_plain);
  // myButtonStylePR.body.main_color = LV_COLOR_MAKE(255, 0, 0);
  // myButtonStylePR.body.grad_color = LV_COLOR_MAKE(0, 0, 255);
  // myButtonStylePR.body.radius = 0;
  // myButtonStylePR.text.color = LV_COLOR_MAKE(255, 255, 255);

  // myButton = lv_btn_create(lv_scr_act(), NULL);                       //create button, lv_scr_act() is deafult screen object
  // lv_obj_set_free_num(myButton, 0);                                   //set button is to 0
  // lv_btn_set_action(myButton, LV_BTN_ACTION_CLICK, btn_click_action); //set function to be called on button click
  // lv_btn_set_style(myButton, LV_BTN_STYLE_REL, &myButtonStyleREL);    //set the relesed style
  // lv_btn_set_style(myButton, LV_BTN_STYLE_PR, &myButtonStylePR);      //set the pressed style
  // lv_obj_set_size(myButton, 200, 50);                                 //set the button size
  // lv_obj_align(myButton, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);         //set the position to top mid

  // myButtonLabel = lv_label_create(myButton, NULL);      //create label and puts it inside of the button
  // lv_label_set_text(myButtonLabel, "Click the Button"); //sets label text

  // myLabel = lv_label_create(lv_scr_act(), NULL);                 //create label and puts it on the screen
  // lv_label_set_text(myLabel, "Button has not been clicked yet"); //sets label text
  // lv_obj_align(myLabel, NULL, LV_ALIGN_CENTER, 10, 0);         //set the position to center

  resetAuton1();
  auton_control->define_auton("auton1", auton1);
  auton_control->select_auton("auton1");
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
