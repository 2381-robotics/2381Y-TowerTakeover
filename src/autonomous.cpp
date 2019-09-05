#include "main.h"
#include "utils/motor_controller.h"
#include "utils/robot/drive/mech_drive.h"
#include "utils/robot/drive/drive.h"


#include "api.h"
#include "auto_drive.h"
#include <math.h>
#include "utils/pid.h"
#include "initialize.h"


/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

Auto_Drive* auto_drive = new Auto_Drive(&mech_drive);

void autonomous() {
  auto_drive->Set_Point_Drive(0,0,0);
}
