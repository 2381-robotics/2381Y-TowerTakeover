#include "main.h"
#include "utils/motor_controller.hpp"
#include "utils/robot/drive/mech_drive.hpp"
#include "utils/robot/drive/drive.hpp"


#include "api.h"
#include "auto_drive.h"
#include <math.h>
#include "utils/pid.h"
#include "globals.hpp"
#include "auton_control.hpp"
#include "utils/structure/auto_sequence.hpp"
#include <vector>
#include<functional>
using namespace std;
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
// Auto_Drive* auto_drive;
// auton_control->load_auton(left_back_sequence)
void autonomous() {
  while(true) {
    auton_control->run();

    pros::delay(20);
  }
};

//operations are stopped once a sync task is reached and is only continued once the sync task has been completed
//multiple operations can be run in one sync task that will be run at the same time
//async task runs all operations at the same time
AutoSequence *auton1 = AutoSequence::FromTasks(
  vector<AutoTask>{
    AutoTask::AsyncTask(
      //Drive to cube.
        [](void) -> void {robot->set_point_drive(100, 0 , 0);}, [](void) -> bool { return true; }),

    AutoTask::AsyncTask(
        [](void) -> void {}, [](void) -> bool { return true; }),
});
