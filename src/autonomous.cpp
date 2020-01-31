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
        robot->set_drive(0, 0, 0, 0);
        angler->Smooth_Angler(3.5);
    },
    [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height) && (abs(angler->Get_Speed() < 20))); });

AutoTask stackTask2 = AutoTask::SyncTask(
    [](void) -> void {
        // intake->Set_Intake(0);
        // robot->set_drive(-30, 40, 0, 0);
        // angler->Smooth_Angler_Auton(1);
        robot->drive->Set_Point_Drive(35, 0, 625);
        intake->Set_Intake(-10);
        // intake->Set_Intake(45);
        // robot->drive->Move_Wheel(30);
        // robot->drive->Set_Point_Drive(30, 0, 1500);
        // robot->set_drive(40, 50, 0, 0);

    },
    [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height/2.5) && (abs(angler->Get_Speed() < 20))); });

AutoTask Turn90DegRight = AutoTask::SyncTask(
    [](void) -> void {
        // intake->Set_Intake(127);
        robot->drive->Set_Point_Drive(0, 0, 1700, 127, 1.5, 0.8);
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); });

    AutoTask Turn90DegLeft = AutoTask::SyncTask(
    [](void) -> void {
        // intake->Set_Intake(127);
        robot->drive->Set_Point_Drive(0, 0, 1700, -127, 1.5, 0.8);
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); });

AutoTask Turn90Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        robot->drive->Set_Point_Drive(0, 0, 1290 , 100, 2.5);
        // pros::lcd::set_text(0, "wa");s
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); robot->drive->Stop();});

// acc like 55 degrees
AutoTask Turn45Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        // 625
        robot->drive->Set_Point_Drive(0, 0, 710 + autonomous_increment, 100, 2.5);
        // pros::lcd::set_text(0, "wa");s
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });

AutoTask InvertTurn45Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        // 625
        robot->drive->Set_Point_Drive(0, 0, 700 + autonomous_increment, -75, 2.5);
        // pros::lcd::set_text(0, "wa");s
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });


AutoTask InvertTurn90Deg = AutoTask::SyncTask(
    [](void) -> void {
        intake->Set_Intake(127);
        robot->drive->Set_Point_Drive(0, 0, 1295 + 650 + autonomous_increment, -90, 2.5);

        // pros::lcd::set_text(0, "wa");s
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); });

AutoTask DeployTask = AutoTask::SyncTask(
    [](void) -> void {
        robot->drive->Set_Point_Drive(127, 0, 700, 0, 1, false, 400, 3);  
        arm->Set_Target(300);
        intake->Set_Intake(-80);
    },
    [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Set_Intake(127); arm->Set_Target(0); });

AutoTask OnSpotDeploy = AutoTask::AutoDelay(
                            500, true, [](void) -> void {
                                arm->Set_Target(300);
                                intake->Set_Intake(-80); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Set_Intake(127); arm->Set_Target(0); });

AutoSequence *  auton1;

AutoSequence *testDriveStraightAuton2 = AutoSequence::FromTasks(
    vector<AutoTask>{
        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         intake->Set_Intake(200);
        //         robot->drive->Set_Point_Drive(40, 0, 5000, 0, 2, 0.8, false, 4000, 1, {0, 0, 0, 0});
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // AutoTask::AutoDelay(10000000),
        OnSpotDeploy,
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200);
                robot->drive->Set_Point_Drive(127, 0, 2750, 0, 2, 0.8, false, 400, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         intake->Set_Intake(127);
        //         robot->drive->Set_Point_Drive(60, 0, 1000, 0, 1, 1, false, 400, 1, {0, 0, 0, 0});
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(80);
                robot->drive->Set_Point_Drive(127, 160, 2100, 0, 1, 1, false, 400, 1, {100, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(127, 90, 700, -20, 20,4, false, 400, 1, {0,60, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // drive straight to reach the cubes
        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         intake->Set_Intake(127);
        //         robot->drive->Set_Point_Drive(127, 0, 1000,0,1,1,false,400,1, {0,60,0,0});
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // slow downs while picking up cubes
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200);
                robot->drive->Set_Point_Drive(40, 0, 2650, 0, 2, 100, false, 2650, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // extend angler a bit

        AutoTask::AutoDelay(200),
        // AutoTask::AsyncTask(
        //     [](void) -> void {
        //         angler->Smooth_Angler(1.8);
        //         intake->Set_Intake(-20);
        //     },
        //     [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height / 3.2)); }),
        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         intake->Set_Intake(60);

        //         robot->drive->Set_Point_Drive(127, 110, 2500, 127, 1, 1, false, 400, 1, {0,70,0,0});
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         robot->drive->Set_Point_Drive(127, 0, 1300);
        //         intake->Set_Intake(-40);
        //         angler->Smooth_Angler(1);
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(60);

                robot->drive->Set_Point_Drive(127, 180, 1100, 127, 2, 1, false, 1050, 1, {100, 0, 127, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(-20);

                robot->drive->Set_Point_Drive(127, 0, 1700, 127, 2, 2, false, 1000, 1, {0, 127, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(-40);
                angler->Smooth_Angler(1);

                robot->drive->Set_Point_Drive(127, 0, 1000, 0, 2, 2, false, 400, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        stackTask,
        AutoTask::AutoDelay(600, true, [](void) -> void {
            robot->set_drive(0, 20, 0, 0);
        }),
        AutoTask::AutoDelay(200),

        AutoTask::SyncTask([](void) -> void {
                // intake->Set_Intake(-40);
                robot->drive->Set_Point_Drive(127, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::AutoDelay(10000000),
    });

// bruh test auton boi
//5 point
AutoSequence *testDriveStraightAuton = AutoSequence::FromTasks(
    vector<AutoTask>{
        // drive straight to reach the cubes
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(127, 0, 1000,0,1,1,false,400,1, {0,60,0,0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // slow downs while picking up cubes
        AutoTask::SyncTask(
            [](void) -> void {
                // intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(60, 0, 2100);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // extend angler a bit

        AutoTask::AutoDelay(200),
        // AutoTask::AsyncTask(
        //     [](void) -> void {
        //         angler->Smooth_Angler(1.8);
        //         intake->Set_Intake(-20);
        //     },
        //     [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height / 3.2)); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(40);
                robot->drive->Set_Point_Drive(127, 110, 2600, 110, 1, 1, false, 400, 1, {0, 70, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(127, 0, 500);
                intake->Set_Intake(-40);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        stackTask,
        AutoTask::AutoDelay(400, true, [](void) -> void {
            robot->set_drive(0, 40, 0, 0);
        }),
        AutoTask::SyncTask([](void) -> void {
                // intake->Set_Intake(-40);
                robot->drive->Set_Point_Drive(127, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::AutoDelay(10000000),
    });

// AutoSequence *invertedSmallSideAuto = auton1 = AutoSequence::FromTasks(
//     vector<AutoTask>{
//         AutoTask::AutoDelay(500, false, [](void)->void{
//             intake->Set_Intake(-80);
//         }, [&](void)->void{
//             return;
//         }, [](void)->void{
//             intake->Set_Intake(127);
//         }),
//         DeployTask,
//         AutoTask::SyncTask([](void) -> void {
//                 intake->Set_Intake(127);
//                 arm->Set_Target(0);
//                 robot->drive->Set_Point_Drive(60, 0, 1700); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
//         // AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),
//         AutoTask::SyncTask(
//             [](void) -> void { // diagonal drive
//                 intake->Set_Intake(127);
//                 robot->drive->Set_Point_Drive(127, -140, 2800 + autonomous_increment);
//                 pros::lcd::set_text(6, "HELLO THERE");
//             },
//             [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
//         AutoTask::SyncTask(
//             [](void) -> void {
//                 intake->Set_Intake(127);
//                 robot->drive->Set_Point_Drive(50, -90, 250, 0, 0.2);
//                 pros::lcd::set_text(6, "HELLO THERE");
//             },
//             [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
//         AutoTask::SyncTask(
//             [](void) -> void {
//                 intake->Set_Intake(127);
//                 robot->drive->Set_Point_Drive(127, 0, 1000, 0, 1, false, 400, 3);
//             },
//             [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
//         AutoTask::SyncTask(
//             [](void) -> void {
//                 intake->Set_Intake(127);
//                 robot->drive->Set_Point_Drive(60, 0, 2000);
//             },
//             [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
//         AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),

//         InvertTurn90Deg,
//         AutoTask::SyncTask( //Diagonal
//             [](void) -> void {
//                 intake->Set_Intake(127);
//                 robot->drive->Set_Point_Drive(127, -87, 3900, 0, 1.5, false, 100);
//             },
//             [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
//         AutoTask::SyncTask(
//             [](void) -> void {
//                 intake->Set_Intake(0);
//                 robot->drive->Set_Point_Drive(40, 0, 200, 0, 3);
//                 // pros::lcd::set_text(0, "HELLO THERE");
//             },
//             [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
//         AutoTask::AutoDelay(300, true, [](void) -> void {
//             intake->Set_Intake(100);
//             robot->set_drive(-30, 50, 0, 0);
//         }),

//         AutoTask::AutoDelay(3000, false, [](void) -> void {
//             intake->Set_Intake(-10);
//             robot->set_drive(-30, 50, 0, 0);
//         }),

//         stackTask,
//         AutoTask::AutoDelay(500),
//         AutoTask::SyncTask([](void) -> void {
//                     intake->Set_Intake(0);
//                     robot->drive->Set_Point_Drive(40, 180, 500); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
//         AutoTask::AutoDelay(10000000),
//     });
AutoSequence* blue5PointAuton;
AutoSequence* straightRedAuton = testDriveStraightAuton2;
AutoSequence *safe6SlowAuton = AutoSequence::FromTasks(
    vector<AutoTask>{
        AutoTask::AutoDelay(100),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(50);
                robot->drive->Set_Point_Drive(100, 90, 2200 + autonomous_increment);
                // pros::lcd::set_text(0, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // AutoTask::AutoDelay(100),

        AutoTask::AutoDelay(900, true, [](void) -> void { robot->drive->Set_Drive(0, -80, 0, 0); }),
        AutoTask::AutoDelay(100),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(100);
                robot->drive->Set_Point_Drive(100, 0, 3500);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // AutoTask::AutoDelay(100s),
        AutoTask::AutoDelay(500, true, [](void) -> void { intake->Set_Intake(100); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(100, 180, 3000);

                // pros::lcd::set_text(0, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        Turn90Deg,
        AutoTask::AutoDelay(100, true, [](void) -> void { robot->set_drive(0, 0, 0, 0); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(40, 30, 1000, 0, 0.3);
                // pros::lcd::set_text(0, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        stackTask,
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(40, 0, 400, 0, 0.3);
                // pros::lcd::set_text(0, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::AutoDelay(500),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(40, 180, 1500, 0, 0.3);
                // pros::lcd::set_text(0, "HELLO THERE");
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::AutoDelay(10000000),

    });

void resetAuton1(int a)
{

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
