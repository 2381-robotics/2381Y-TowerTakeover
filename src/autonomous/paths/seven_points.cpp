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
 DeployTask(), // Outtakes and raises arm to deploy
        AutoTimer::AutoDelay(500), // Wait 500 milliseconds to let tray settle

         AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200); // Set Intake to max speed
                robot->drive->Set_Point_Drive(127, 0, 2300, 0, 2, 0.8, true, 400, 1, {0, 0, 0, 0}); // At the same time, drive forward towards the first row of cubes
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(80); // slow down intake
                robot->drive->Set_Point_Drive(127, 160, 1600, 0, 1, 1, false, 400, 1, {100, 0, 0, 0}); // Drive diagonally 200 deg backwards
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(127, 90, 700, 0, 20, 4, true, 500, 1, {0, 60, 0, 0}); // Diagonal strafe turns into a horizontal strafe with deacceleration 
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // Drive forward again to pick up 4 cubes at max speed
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200);
                robot->drive->Set_Point_Drive(100, 0, 3200, 0, 2, 100, true, 2600, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        
        // Wait a bit for last cube to be intaked
        AutoTask::AutoDelay(100),

        //Drive backwards at max speed
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(60);
                robot->drive->Set_Point_Drive(127, 180, 3600, 0, 2, 0.8, true, 2100, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        // Turn towards goal zone, extend angler during the turn
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(-15);  
                robot->drive->Set_Point_Drive(50, 100, 1700, 127, 2, 2, false, 1400, 1, {0, 0, 0, 0});
                angler->Set_Target(angler->_max_height * 0.4);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // Drive into the goal zone, continue angler slow down the stack
        AutoTask::SyncTask(
            [](void) -> void {

                angler->Smooth_Angler(0.7);
                // Outtake at a slow speed
                intake->Set_Intake(-15);
                robot->drive->Set_Point_Drive(80, 0, 450, 0, 2, 2, false, 175, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // Calls Stack Task - increments angler until max is reached
        StackTask(),
        // Slowly drive into the goal zone to complete the stack for 400 ms
        AutoTask::AutoDelay(400, true, [](void) -> void {
            robot->set_drive(0, 30, 0, 0);
        }),
        // Pause 100 milliseconds
        AutoTask::AutoDelay(200),

        // Drive backwards and outtake to complete the stack
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-40);
                robot->drive->Set_Point_Drive(60, 180, 800);
                angler->Set_Target(0); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

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
                robot->drive->Set_Point_Drive(127, 0, 2300, 0, 2, 0.8, true, 400, 1, {0, 0, 0, 0}); // At the same time, drive forward towards the first row of cubes
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(80); // slow down intake
                robot->drive->Set_Point_Drive(127, 200, 1600, 0, 1, 1, false, 400, 1, {-100, 0, 0, 0}); // Drive diagonally 200 deg backwards
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(127, -90, 700, 0, 20, 4, true, 500, 1, {0, 60, 0, 0}); // Diagonal strafe turns into a horizontal strafe with deacceleration 
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // Drive forward again to pick up 4 cubes at max speed
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200);
                robot->drive->Set_Point_Drive(100, 0, 3200, 0, 2, 100, true, 2600, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        
        // Wait a bit for last cube to be intaked
        AutoTask::AutoDelay(100),

        //Drive backwards at max speed
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(60);
                robot->drive->Set_Point_Drive(127, 180, 3600, 0, 2, 0.8, true, 2100, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        // Turn towards goal zone, extend angler during the turn
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(-15);  
                robot->drive->Set_Point_Drive(50, -100, 1700, -127, 2, 2, false, 1400, 1, {0, 0, 0, 0});
                angler->Set_Target(angler->_max_height * 0.4);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // Drive into the goal zone, continue angler slow down the stack
        AutoTask::SyncTask(
            [](void) -> void {

                angler->Smooth_Angler(0.7);
                // Outtake at a slow speed
                intake->Set_Intake(-15);
                robot->drive->Set_Point_Drive(80, 0, 400, 0, 2, 2, false, 175, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // Calls Stack Task - increments angler until max is reached
        StackTask(),
        // Slowly drive into the goal zone to complete the stack for 400 ms
        AutoTask::AutoDelay(400, true, [](void) -> void {
            robot->set_drive(0, 30, 0, 0);
        }),
        // Pause 100 milliseconds
        AutoTask::AutoDelay(200),

        // Drive backwards and outtake to complete the stack
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-40);
                robot->drive->Set_Point_Drive(60, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::AutoDelay(10000000),

    });
