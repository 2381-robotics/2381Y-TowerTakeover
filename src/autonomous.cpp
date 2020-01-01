#include "main.h"
#include "robot/control/motor_controller.hpp"
#include "robot/modules/drive/mech_drive.hpp"
#include "robot/modules/drive/drive.hpp"

#include "api.h"
#include <math.h>
#include "robot/control/pid.hpp"
#include "globals.hpp"
#include "autonomous/auton_control.hpp"
#include "autonomous/auto_timer.hpp"
#include "autonomous/auto_sequence.hpp"
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

AutoTask stackTask = AutoTask::SyncTask(
    [](void) -> void {
        angler->Smooth_Angler(1);
        robot->set_drive(0,0,0,0);
        intake->Set_Intake(0);
    },
    [](void) -> bool {return ((angler->Get_Height() >= angler->_max_height) && (abs(angler->Get_Speed() < 20)));});

AutoTask Turn90Deg = AutoTask::SyncTask(
             [](void) -> void {
                 intake->Set_Intake(0);
                 robot->set_point_drive(0, 0, 1250 + autonomous_increment, 100, 2);
                 // pros::lcd::set_text(0, "wa");s
             },
             [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });

AutoSequence *auton1;

void resetAuton1() {
    auton1 = AutoSequence::FromTasks(
        vector<AutoTask>{

            AutoTask::SyncTask(
                [](void) -> void {
                    intake->Set_Intake(127);
                    robot->set_point_drive(80, 0, 2700);
                },
                [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),
            AutoTask::SyncTask(
                [](void) -> void {
                    intake->Set_Intake(50);
                    robot->set_point_drive(127, 140, 2800 + autonomous_increment);
                    pros::lcd::set_text(6, "HELLO THERE");
                },
                [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            AutoTask::SyncTask(
                [](void) -> void {
                    intake->Set_Intake(50);
                    robot->set_point_drive(50, 90, 150, 0 , 0.2);
                    pros::lcd::set_text(6, "HELLO THERE");
                },
                [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            AutoTask::SyncTask(
                [](void) -> void {
                    intake->Set_Intake(127);
                    robot->set_point_drive(80, 0, 2700);
                },
                [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),
            Turn90Deg,
            AutoTask::SyncTask(
                [](void) -> void {
                    intake->Set_Intake(0);
                    robot->set_point_drive(127, 85, 2800);
                },
                [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

            stackTask,
            AutoTask::SyncTask(
                [](void) -> void {
                    intake->Set_Intake(0);
                    robot->set_point_drive(40, 0, 400, 0, 0.5);
                    // pros::lcd::set_text(0, "HELLO THERE");
                },
                [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

            AutoTask::AutoDelay(500),
            AutoTask::SyncTask(
                [](void) -> void {
                    intake->Set_Intake(0);
                    robot->set_point_drive(40, 180, 1500, 0, 0.5);
                    // pros::lcd::set_text(0, "HELLO THERE");
                },
                [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

            // AutoTask::AutoDelay(100),
            // AutoTask::SyncTask(
            //     [](void) -> void {
            //         intake->Set_Intake(50);
            //         robot->set_point_drive(100, 90, 2200 +autonomous_increment);
            //         // pros::lcd::set_text(0, "HELLO THERE");
            //     },
            //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            // // AutoTask::AutoDelay(100),

            // AutoTask::AutoDelay(900, true, [](void)->void{robot->drive->Set_Drive(0,-80,0,0);}),
            // AutoTask::AutoDelay(100),
            // AutoTask::SyncTask(
            //     [](void) -> void {
            //         intake->Set_Intake(100);
            //         robot->set_point_drive(100, 0, 3500);
            //     },
            //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            // // AutoTask::AutoDelay(100s),
            // AutoTask::AutoDelay(500, true, [](void) -> void { intake->Set_Intake(100); }),

            // AutoTask::SyncTask(
            //     [](void) -> void {
            //         intake->Set_Intake(0);
            //         robot->set_point_drive(100, 180, 3000);

            //         // pros::lcd::set_text(0, "HELLO THERE");
            //     },
            //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

            // Turn90Deg,
            // AutoTask::AutoDelay(100, true, [](void) -> void { robot->set_drive(0, 0, 0, 0); }),

            // AutoTask::SyncTask(
            //     [](void) -> void {
            //         intake->Set_Intake(0);
            //         robot->set_point_drive(40, 30, 1000, 0 , 0.5);
            //         // pros::lcd::set_text(0, "HELLO THERE");
            //     },
            //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            // stackTask,
            // AutoTask::SyncTask(
            //     [](void) -> void {
            //         intake->Set_Intake(0);
            //         robot->set_point_drive(40, 0, 400, 0, 0.5);
            //         // pros::lcd::set_text(0, "HELLO THERE");
            //     },
            //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

            // AutoTask::AutoDelay(500),
            // AutoTask::SyncTask(
            //     [](void) -> void {
            //         intake->Set_Intake(0);
            //         robot->set_point_drive(40, 180, 1500, 0, 0.5);
            //         // pros::lcd::set_text(0, "HELLO THERE");
            //     },
            //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

            AutoTask::AutoDelay(10000000),

        }

    );
}


void autonomous()
{


    while (true)
    {
        auton_control->run();
        pros::delay(20);
    }

};
//operations are stopped once a sync task is reached and is only continued once the sync task has been completed
//multiple operations can be run in one sync task that will be run at the same time
//async task runs all operations at the same time
