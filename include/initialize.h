#include "main.h"
#include "ports.h"
#include "opcontrol.h"

#include "main.h"
#include "ports.h"

#include "utils/pid.h"
#include "utils/motor_controller.h"
#include "utils/robot/drive/mech_drive.h"

#include "auto_drive.h"


#include <list>
#include <map>

double kp;
double ki;
double kd;
Pid* lift_pid;


double left_back_pid_values;
double right_back_pid_value;
double left_front_pid_values;
double right_front_pid_values;

pros::Motor left_front_motor (LEFT_FRONT_MOTOR_PORT, false);
pros::Motor left_back_motor (LEFT_BACK_MOTOR_PORT, false);
pros::Motor right_front_motor (RIGHT_FRONT_MOTOR_PORT, true);
pros::Motor right_back_motor (RIGHT_BACK_MOTOR_PORT, true);
double master_pid_values;

Motor_Controller* left_front_motor_controller;
Motor_Controller* left_back_motor_controller;
Motor_Controller* right_front_motor_controller;
Motor_Controller* right_back_motor_controller;

Mech_Drive* mech_drive;
Auto_Drive* auto_drive;
