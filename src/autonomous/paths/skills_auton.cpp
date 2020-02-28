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
       // deplay task called to run outake and flip out tray
        DeployTask(),
        // driving forward and intaking cubes in the second row from goal zone
         AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200);
                robot->drive->Set_Point_Drive(127, 0, 2300, 0, 2, 0.7, true, 400, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // driving backwards diagonally to the left and alligning against the wall
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(80);
                robot->drive->Set_Point_Drive(127, 200, 1600, 0, 1, 1, false, 400, 1, {-100, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // strafing to the left and alligning with 1st row for 4 cubes beside the goal zone
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(80, -90, 700, 0, 20, 4, true, 500, 1, {0, 60, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // picking up cubes in the 1st row
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(200);
                robot->drive->Set_Point_Drive(80, 0, 3200, 0, 2, 100, true, 2600, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        AutoTask::AutoDelay(200), //not sure if this is efficent or not!
        // driving backwards towards the wall 
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(60);
                robot->drive->Set_Point_Drive(127, 180, 3500, 0, 2, 0.8, true, 2100, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void {}),
        // outtaking slightly and turning left into the goal zone
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(-10);
                robot->drive->Set_Point_Drive(40, -100, 1600, -127, 2, 2, false, 1400, 1, {0, 0, 0, 0});
                angler->Set_Target(angler->_max_height/3);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // pre-extending tray to save time for stacking, and moving close into the zone
        AutoTask::SyncTask(
            [](void) -> void {
                angler->Smooth_Angler(0.7);
                intake->Set_Intake(-10);
                robot->drive->Set_Point_Drive(65, 0, 350, 0, 2, 2, false, 175, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // calling stackTask to stack all the cubes in the goalzone
        StackTask(),
        AutoTask::AutoDelay(400, true, [](void) -> void {
            robot->set_drive(0, 25, 0, 0);
        }),
        AutoTask::AutoDelay(100),
        // outtaking cubes and driving backwards away from the stack
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-40);
                // change 800 so that we are in line w the small tower
                robot->drive->Set_Point_Drive(60, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // adding a delay so that the bot does not move after finishing the stack if there is extra time remaining 
        
        // paralel to the wall where we started and facing goalzone



// bruh 




        // 1. Strafe towards tower (right)
         AutoTask::SyncTask([](void) -> void {
                robot->drive->Set_Point_Drive(60, 90, 800); }, 
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // 2. 90 degree right turn
        AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(0, 0, 1700, 127, 1.5, 0.8);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // 3. wall align using ultras while driving forward to tower
        // 4. intake cube
         AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(100);
                robot->drive->Set_Point_Drive(127, 0, 2350, 0, 2, 0.7, true, 400, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        
        // 5. Arm task (1,2) based on mid towers
                // 1. Move back 500 
                // 2. increment arm
                // 3. move forard
                // 4. outake 
        // ArmTaskSmall(),

// arm task small

        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(100);
            robot->drive->Set_Point_Drive(100, 180, 400);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
         AutoTask::SyncTask(
            [](void) -> void {
            arm->Arm_Macro(1); //low tower
            },
            [](void) -> bool {return ((arm->Get_Height() >= 1550));}),
        AutoTask::SyncTask(
            [](void) -> void {
            robot->drive->Set_Point_Drive(100, 0, 200);
            intake->Set_Intake(-100);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
         AutoTask::SyncTask(
            [](void) -> void {
            arm->Arm_Macro(0); //low tower
            },
            [](void) -> bool {return ((arm->Get_Height() <= 100));}),
        

        // 6. move back like 2000
        AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(100, 180, 1500);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        
        // 7. turn rught 90 
         AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(0, 0, 1700, 127, 1.5, 0.8);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // 8. move into mid tower
        // 9. intake cube
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(100);
                robot->drive->Set_Point_Drive(127, 0, 1000, 0, 2, 0.7, true, 400, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // 10. tower
        // ArmTaskMid(),


        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(100);
            robot->drive->Set_Point_Drive(100, 180, 400);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
         AutoTask::SyncTask(
            [](void) -> void {
            arm->Arm_Macro(2); //low tower
            },
            [](void) -> bool {return ((arm->Get_Height() >= 2250));}),
        AutoTask::SyncTask(
            [](void) -> void {
            robot->drive->Set_Point_Drive(100, 0, 200);
            intake->Set_Intake(-100);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
         AutoTask::SyncTask(
            [](void) -> void {
            arm->Arm_Macro(0); //low tower
            },
            [](void) -> bool {return ((arm->Get_Height() <= 100));}),




        // 11. move back and finish 
        AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(127, 180, 1000, 0, 2, 0.7, true, 400, 1, {0, 0, 0, 0});//increase if needed
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        
        
        // 1st sequence done (1 stack (7-8 cubes) 2 towers (21 - 24 points))
        // 2nd sequence (2nd stack (3 cubes) (9 points))
        
        //1. turn left 
        AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(0, 0, 1700, -127, 1.5, 0.8);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        //2. drive forard and intake all the way almost to the wall
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(100);
                robot->drive->Set_Point_Drive(127, 0, 5200, 0, 2, 0.7, true, 400, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        //3. do a 90 to the left and strafe right into wall 
         AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(0, 0, 1700, -127, 1.5, 0.8);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // strafe into wall
        AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(100, 90, 500);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        //4.  drive forward to stack 
        AutoTask::SyncTask(
            [](void) -> void {
                robot->drive->Set_Point_Drive(100, 0, 1000);
            },
        [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        // 5. stack and pull back 
        StackTask(),
        AutoTask::AutoDelay(400, true, [](void) -> void {
            robot->set_drive(0, 25, 0, 0);
        }),
        AutoTask::AutoDelay(100),
        // outtaking cubes and driving backwards away from the stack
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-40);
                // change 800 so that we are in line w the small tower
                robot->drive->Set_Point_Drive(60, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),        


        AutoTask::AutoDelay(10000000),

    });