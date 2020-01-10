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
        // intake->Set_Intake(0);
        // robot->set_drive(-30, 40, 0, 0);
        angler->Smooth_Angler(1);
        // 25 0 250
        robot->set_point_drive(40, 0, 350); 

        // robot->set_point_drive(20, 0, 500);
        // intake->Set_Intake(-20);
        // robot->drive->Move_Wheel(30);
        // robot->set_point_drive(30, 0, 1500);
        // robot->set_drive(40, 50, 0, 0);

    },
    [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height) && (abs(angler->Get_Speed() < 20))); });

AutoTask stackTask2 = AutoTask::SyncTask(
    [](void) -> void {
        // intake->Set_Intake(0);
        // robot->set_drive(-30, 40, 0, 0);
        // angler->Smooth_Angler_Auton(1);
        robot->set_point_drive(35, 0, 625);
        intake->Set_Intake(-10);
        // intake->Set_Intake(45);
        // robot->drive->Move_Wheel(30);
        // robot->set_point_drive(30, 0, 1500);
        // robot->set_drive(40, 50, 0, 0);

    },
    [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height/2.5) && (abs(angler->Get_Speed() < 20))); });


AutoTask Turn90Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        robot->set_point_drive(0, 0, 1290 + autonomous_increment, 100, 2.5);
        // pros::lcd::set_text(0, "wa");s
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });

// acc like 55 degrees
AutoTask Turn45Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        // 625
        robot->set_point_drive(0, 0, 710 + autonomous_increment, 100, 2.5);
        // pros::lcd::set_text(0, "wa");s
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });

AutoTask InvertTurn90Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        robot->set_point_drive(0, 0, -1250  - autonomous_increment, 100, 2.5);
        // pros::lcd::set_text(0, "wa");s
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });


AutoTask DeployTask = AutoTask::SyncTask(
    [](void) -> void {
        robot->set_point_drive(50, 0, 100, 0, 1, false, 400, 3);  
        arm->Set_Target(500);

    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });

AutoSequence *auton1;

void resetAuton1(int choice)
{
    auton1 = AutoSequence::FromTasks(
        vector<AutoTask>{
            // AutoTask::AutoDelay(500, false, [](void)->void{
            //     intake->Set_Intake(-80);
            // }, [&](void)->void{
            //     return;
            // }, [](void)->void{
            //     intake->Set_Intake(127);
            // }),
            // DeployTask,

            // AutoTask::SyncTask([](void) -> void {
            //         intake->Set_Intake(127);
            //         arm->Set_Target(0);
            //         robot->set_point_drive(60, 0, 1700); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
                        // // AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),
                        // AutoTask::SyncTask(
                        //     [](void) -> void { // diagonal drive
                        //         intake->Set_Intake(127);
                        //         robot->set_point_drive(127, 140, 2800 + autonomous_increment);
                        //         pros::lcd::set_text(6, "HELLO THERE");
                        //     },
                        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
                        // AutoTask::SyncTask(
                        //     [](void) -> void {
                        //         intake->Set_Intake(127);
                        //         robot->set_point_drive(50, 90, 250, 0, 0.2);
                        //         pros::lcd::set_text(6, "HELLO THERE");
                        //     },
                        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
                        // AutoTask::SyncTask(
                        //     [](void) -> void {
                        //         intake->Set_Intake(127);
                        //         robot->set_point_drive(127, 0, 1000, 0, 1, false, 400, 3);
                        //     },
                            // [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            AutoTask::SyncTask(
                [](void) -> void {
                    intake->Set_Intake(127);
                    robot->set_point_drive(70, 0, 2900);
                },
                [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),

            Turn90Deg,

//strafe to wall
             AutoTask::AutoDelay(2200, true, [](void) -> void {
                 intake->Set_Intake(127);
                 robot->set_point_drive(100, 90, 4190, 0, 1.5, false, 100);
            }),
           
//strafe a bit 
            AutoTask::SyncTask(
                [](void) -> void {
                    robot->set_point_drive(127, -90, 395, 0, 1.5, false, 100);
                },
                [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),       

            Turn45Deg,
            // AutoTask::AutoDelay(1550, true, [](void) -> void {
            //     intake->Set_Intake(0);//590
            //     robot->set_point_drive(127, -90, 850, 0, 1.5, false, 100);
            // }),

// diagonal strafe
            AutoTask::SyncTask( //Diagonal 
                [](void) -> void {
                    intake->Set_Intake(0);//590
                    robot->set_point_drive(127, -90, 850, 0, 1.5, false, 100);
                },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

// starting intake
            AutoTask::AutoDelay(300, true, [](void) -> void {
                intake->Set_Intake(100);
                robot->set_drive(0, 30, 0, 0);
            }),

// reverse intake
           AutoTask::AutoDelay(800, true, [](void) -> void {
                robot->set_drive(0, 40, 0, 0);
                    // robot->set_point_drive(127, 0, 625);
                 intake->Set_Intake(-15);
            }),
            stackTask,

            AutoTask::AutoDelay(500),
            AutoTask::SyncTask([](void) -> void {
                    intake->Set_Intake(0);
                    robot->set_point_drive(50, 180, 1200); 
                    }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
            AutoTask::AutoDelay(10000000),
        }
    );
}

AutoSequence *invertedSmallSideAuto = auton1 = AutoSequence::FromTasks(
    vector<AutoTask>{
        AutoTask::AutoDelay(500, false, [](void)->void{
            intake->Set_Intake(-80);
        }, [&](void)->void{
            return;
        }, [](void)->void{
            intake->Set_Intake(127);
        }),
        DeployTask,
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(127);
                arm->Set_Target(0);
                robot->set_point_drive(60, 0, 1700); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),
        AutoTask::SyncTask(
            [](void) -> void { // diagonal drive
                intake->Set_Intake(127);
                robot->set_point_drive(127, -140, 2800 + autonomous_increment);
                pros::lcd::set_text(6, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->set_point_drive(50, -90, 250, 0, 0.2);
                pros::lcd::set_text(6, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->set_point_drive(127, 0, 1000, 0, 1, false, 400, 3);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->set_point_drive(60, 0, 2000);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),

        InvertTurn90Deg,
        AutoTask::SyncTask( //Diagonal
            [](void) -> void {
                intake->Set_Intake(127);
                robot->set_point_drive(127, -87, 3900, 0, 1.5, false, 100);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->set_point_drive(40, 0, 200, 0, 3);
                // pros::lcd::set_text(0, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::AutoDelay(300, true, [](void) -> void {
            intake->Set_Intake(100);
            robot->set_drive(-30, 50, 0, 0);
        }),

        AutoTask::AutoDelay(3000, false, [](void) -> void {
            intake->Set_Intake(-10);
            robot->set_drive(-30, 50, 0, 0);
        }),

        stackTask,
        AutoTask::AutoDelay(500),
        AutoTask::SyncTask([](void) -> void {
                    intake->Set_Intake(0);
                    robot->set_point_drive(40, 180, 500); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::AutoDelay(10000000),
    });

AutoSequence *safe6SlowAuton = AutoSequence::FromTasks(
    vector<AutoTask>{
        AutoTask::AutoDelay(100),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(50);
                robot->set_point_drive(100, 90, 2200 + autonomous_increment);
                // pros::lcd::set_text(0, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // AutoTask::AutoDelay(100),

        AutoTask::AutoDelay(900, true, [](void) -> void { robot->drive->Set_Drive(0, -80, 0, 0); }),
        AutoTask::AutoDelay(100),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(100);
                robot->set_point_drive(100, 0, 3500);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // AutoTask::AutoDelay(100s),
        AutoTask::AutoDelay(500, true, [](void) -> void { intake->Set_Intake(100); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->set_point_drive(100, 180, 3000);

                // pros::lcd::set_text(0, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        Turn90Deg,
        AutoTask::AutoDelay(100, true, [](void) -> void { robot->set_drive(0, 0, 0, 0); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->set_point_drive(40, 30, 1000, 0, 0.5);
                // pros::lcd::set_text(0, "HELLO THERE");
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
        AutoTask::AutoDelay(10000000),

    });

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
