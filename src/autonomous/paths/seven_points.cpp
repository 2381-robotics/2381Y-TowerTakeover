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
#include "autonomous/global_sequences.hpp"

using namespace std;
using namespace Auton;

AutoSequence *Auton::AT_Red7 = AutoSequence::FromTasks(
    vector<AutoTask>{

        // extend angler a bit
        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         intake->Set_Intake(200);
        //         robot->drive->Set_Point_Drive(40, 0, 5000, 0, 2, 0.8, false, 4000, 1, {0, 0, 0, 0});
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // AutoTask::AutoDelay(10000000),
        // StackTask(),
        DeployTask(),
        AutoTimer::AutoDelay(500),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200);
                // 3050
                robot->drive->Set_Point_Drive(127, 0, 2300, 0, 2, 0.7, true, 400, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(80);
                // 2150
                robot->drive->Set_Point_Drive(127,160, 1600, 0, 1, 1, false, 400, 1, {100, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                //700
                robot->drive->Set_Point_Drive(80, 90, 700, 0, 20, 4, true, 500, 1, {0, 60, 0, 0});
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
                robot->drive->Set_Point_Drive(80, 0, 3200, 0, 2, 100, true, 2600, 1, {0, 0, 0, 0});
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
                robot->drive->Set_Point_Drive(127, 180, 3500, 0, 2, 0.8, true, 2100, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(-10);
                robot->drive->Set_Point_Drive(40, 100, 1600, 127, 2, 2, false, 1400, 1, {0, 0, 0, 0});
                angler->Set_Target(angler->_max_height / 3);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                angler->Smooth_Angler(0.7);

                intake->Set_Intake(-10);
                robot->drive->Set_Point_Drive(65, 0, 350, 0, 2, 2, false, 175, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        StackTask(),
        AutoTask::AutoDelay(400, true, [](void) -> void {
            robot->set_drive(0, 25, 0, 0);
        }),
        AutoTask::AutoDelay(100),

        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-40);
                robot->drive->Set_Point_Drive(60, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::AutoDelay(10000000),

    });

// blue 7 point

AutoSequence *Auton::AT_Blue7 = AutoSequence::FromTasks(
    vector<AutoTask>{
        DeployTask(), // Outtakes and raises arm to deploy
        AutoTimer::AutoDelay(500), // Wait 500 milliseconds to let tray settle

         AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200); // Set Intake to max speed
                robot->drive->Set_Point_Drive(127, 0, 2300, 0, 2, 0.7, true, 400, 1, {0, 0, 0, 0}); // At the same time, drive forward towards the first row of cubes
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }},

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(80);
                // 2150
                robot->drive->Set_Point_Drive(127, 200, 1600, 0, 1, 1, false, 400, 1, {-100, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                //700
                robot->drive->Set_Point_Drive(80, -90, 700, 0, 20, 4, true, 500, 1, {0, 60, 0, 0});
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
                robot->drive->Set_Point_Drive(90, 0, 3200, 0, 2, 100, true, 2600, 1, {0, 0, 0, 0});
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
                robot->drive->Set_Point_Drive(127, 180, 3600, 0, 2, 0.8, true, 2100, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(-15);
                robot->drive->Set_Point_Drive(50, -100, 1700, -127, 2, 2, false, 1400, 1, {0, 0, 0, 0});
                angler->Set_Target(angler->_max_height*0.4);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                angler->Smooth_Angler(0.7);

                intake->Set_Intake(-15);
                robot->drive->Set_Point_Drive(75, 0, 400, 0, 2, 2, false, 175, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        StackTask(),
        AutoTask::AutoDelay(400, true, [](void) -> void {
            robot->set_drive(0, 25, 0, 0);
        }),
        AutoTask::AutoDelay(100),

        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-40);
                robot->drive->Set_Point_Drive(60, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::AutoDelay(10000000),

    });
