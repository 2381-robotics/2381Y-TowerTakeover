#include "main.h"
#include "utils/motor_controller.hpp"
#include "utils/robot/drive/mech_drive.hpp"
#include "utils/robot/drive/drive.hpp"

#include "api.h"
#include <math.h>
#include "utils/pid.h"
#include "globals.hpp"
#include "auton_control.hpp"
#include "utils/structure/auto_sequence.hpp"
#include <vector>
#include <functional>
using namespace std;
// /**
//  * Runs the user autonomous code. This function will be started in its own task
//  * with the default priority and stack size whenever the robot is enabled via
//  * the Field Management System or the VEX Competition Switch in the autonomous
//  * mode. Alternatively, this function may be called in initialize or opcontrol
//  * for non-competition testing purposes.
//  *
//  * If the robot is disabled or communications is lost, the autonomous task
//  * will be stopped. Re-enabling the robot will restart the task, not re-start it
//  * from where it left off.
//  */
// // Auto_Drive* auto_drive;
// int times =0 ;
int _count = 0;
AutoSequence *auton1 = AutoSequence::FromTasks(
    vector<AutoTask>{
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(100);
                robot->set_point_drive(100, 0, 4000);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); pros::lcd::set_text(1, "Drive initialized"); }),
        // AutoTask::AutoDelay(4000, true),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->set_point_drive(-100, 0, 3000); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); pros::lcd::set_text(1, "Drive initialized"); }),
        AutoTask::SyncTask(
            [](void) -> void { robot->drive->Set_Turn(60, 70, 2000); }, [](void) -> bool { return !robot->drive->get_running(); }, [](void) -> void { robot->drive->Reset_Point(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->set_point_drive(100, 0, 3000); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); pros::lcd::set_text(1, "Drive initialized"); })
        
    });

void autonomous()
{
    auton_control->define_auton("auton1", auton1);
    auton_control->select_auton("auton1");
    pros::lcd::set_text(0, "Void Autonomous Function");

    while (true)
    {
        auton_control->run();
        pros::delay(20);
    }

    // while (true)
    // {

    //     // intake->Set_Intake(100);
    //     // robot->drive->Set_Turn(100, 0, 100);

    //     // robot->set_point_drive(100, 160, 500);

    //     //   auton_control->run();

    //     //   pros::delay(20);
    //   }
};
// //operations are stopped once a sync task is reached and is only continued once the sync task has been completed
// //multiple operations can be run in one sync task that will be run at the same time
// //async task runs all operations at the same time
