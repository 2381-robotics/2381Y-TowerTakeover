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

// double 0
// {
//         double leftDistance = ultra_left.get_value();
//         double rightDistance = ultra_right.get_value();
//         return 1.5 * (abs(rightDistance - leftDistance) / (rightDistance - leftDistance) * 70 * pow(abs(rightDistance - leftDistance) / 70, 0.5));
// }
AutoSequence *Auton::AT_Skills = AutoSequence::FromTasks(
    vector<AutoTask>{
     DeployTask(),              // Outtakes and raises arm to deploy
        AutoTimer::AutoDelay(500), // Wait 500 milliseconds to let tray settle

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200); // Set Intake to max speed
                // 2350
                robot->drive->Set_Point_Drive(127, 0, 2350, 0, 2, 0.8, true, 400, 1, {0, 0, 0, 0}); // At the same time, drive forward towards the first row of cubes
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
                // 750
                robot->drive->Set_Point_Drive(127, -90, 800, 0, 20, 4, true, 00, 1, {0, 60, 0, 0}); // Diagonal strafe turns into a horizontal strafe with deacceleration
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // Drive forward again to pick up 4 cubes at max speed
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200);
                robot->drive->Set_Point_Drive(60, 0, 3000, 0, 2, 100, true, 2600, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        // Wait a bit for last cube to be intaked
        AutoTask::AutoDelay(100),

        //Drive backwards at max speed
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(60);
                robot->drive->Set_Point_Drive(100, 180, 3400, 0, 2, 0.8, true, 2100, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),

        // Turn towards goal zone, extend angler during the turn
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Index_Intake(-50, 550);

                robot->drive->Set_Point_Drive(50, -100, 1700, -127, 2, 2, false, 1400, 1, {0, 0, 0, 0});
                angler->Set_Target(angler->_max_height * 0.43);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void {intake->Reset_Point();  robot->drive->Reset_Point(); }, [](void) -> void {}),

        // Drive into the goal zone, continue angler slow down the stack
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Index_Intake(-50, 550);

                angler->Smooth_Angler(0.7);
                // Outtake at a slow speed
                intake->Set_Intake(-15);
                robot->drive->Set_Point_Drive(80, 0, 475, 0, 2, 2, false, 250, 1, {0, 0, 0, 0});
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
                // 800
                angler->Set_Target(0);
                robot->drive->Set_Point_Drive(60, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        // paralel to the wall where we started and facing goalzone









// bruh 
    AutoTask::SyncTask(
            [](void) -> void {
                // 1000
                robot->drive->Set_Point_Drive(100, 180, 850);
                intake->Set_Intake(0);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        

        AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(0, 0, 1500, 127, 1.5, 1.5);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        
      AutoTimer::AutoDelay(1000, true, 
      [](void) -> void {
          robot->drive->Set_Drive(0,0,ultra_finder->Ultra_Angle(), 0);
      }),
        // 1. Strafe towards tower (right)
       
      // 3. wall align using ultras while driving forward to tower
        // 4. intake cube
         AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(127, 0, 3300, 0, 2, 0.7, false, 400, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        
        // 5. Arm task (1,2) based on mid towers
                // 1. Move back 500 
                // 2. increment arm
                // 3. move forard
                // 4. outake 
        // ArmTaskSmall(),


// driving forward and intaking cube

        AutoTask::SyncTask(
            [](void) -> void {
            intake->Set_Intake(100);
            robot->drive->Set_Point_Drive(100, 180, 400, 0, 10, 10);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

// indexing bottom cube  
      AutoTimer::AutoDelay(1000, true, 
      [](void) -> void {
          intake->Index_Intake(-50,400);
      }, [](void) -> void {
          intake->Reset_Point();
      }),
    //   raising arm for low tower
         AutoTask::SyncTask(
            [](void) -> void {
            intake->Set_Intake(-18);
            arm->Arm_Macro(1); //low tower
            },
            [](void) -> bool {return ((arm->Get_Height() >= 1575));}),

// jank MAKE THIS SHIT WORK
        AutoTask::SyncTask(
            [](void) -> void {
            robot->drive->Set_Point_Drive(100, 0, 400, 0, 100, 100);
            intake->Set_Intake(-50);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
         
        //  auto delay for tower outake
        
        //  AutoTask::AutoDelay(1000, true, [](void) -> void {
        //     intake->Set_Intake(-50);
        //         robot->drive->Set_Point_Drive(127, 180, 2500, 0, 2, 0.7, false, 400, 1, {0, 0, 0, 0});
        // }),
         
         
         AutoTask::SyncTask(
            [](void) -> void {
            arm->Set_Target(0); //reset positon 
            },
            [](void) -> bool {return ((arm->Get_Height() <= 100));}),
        

        //CHANGE THE DRIVE BACK VALUES HERE
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(127, 180, 2500, 0, 2, 0.7, false, 400, 1, {0, 0, 0, 0});
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        
        // CHANGE THE 90 DEGREE TURN HERE
         AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(0, 0, 1500, 127, 1.5, 1.5);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        
    //      AutoTimer::AutoDelay(1000, true, 
    //   [](void) -> void {
    //       robot->drive->Set_Drive(0,0,ultra_finder->Ultra_Angle(), 0);
    //   }),
        
        // CHANGE THE DRIVE FORWARD HERE
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(127, 0, 1300, 0, 2, 0.7, false, 400, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // 10. tower
        // ArmTaskMid(),
        AutoTask::SyncTask(
            [](void) -> void {
            intake->Set_Intake(127);
            robot->drive->Set_Point_Drive(100, 180, 400, 0, 10, 10);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

// indexing bottom cube  
      AutoTimer::AutoDelay(1000, true, 
      [](void) -> void {
          intake->Index_Intake(-50,400);
      }, [](void) -> void {
          intake->Reset_Point();
      }),
    //   raising arm for low tower
         AutoTask::SyncTask(
            [](void) -> void {
            intake->Set_Intake(-18);
            arm->Arm_Macro(2); //low tower
            },
            [](void) -> bool {return ((arm->Get_Height() >= 2275));}),

        // AutoTask::SyncTask(
        //     [](void) -> void {
        //     robot->drive->Set_Point_Drive(100, 0, 400, 0, 100, 100);
        //     intake->Set_Intake(-50);
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
         
        //  auto delay for tower outake
        //  AutoTask::AutoDelay(1000, true, [](void) -> void {
        //     intake->Set_Intake(-50);
        // }),
         
         
        //  MAKE THIS SHIT WORK
           AutoTask::SyncTask(
            [](void) -> void {
            robot->drive->Set_Point_Drive(100, 0, 400, 0, 100, 100);
            intake->Set_Intake(-50);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
         

        // 6. move back like 2000
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(127, 180, 500, 0, 2, 0.7, false, 300, 1, {0, 0, 0, 0});
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        

        AutoTask::AutoDelay(10000000),

    });