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
        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         intake->Set_Intake(200);
        //         robot->drive->Set_Point_Drive(40, 0, 5000, 0, 2, 0.8, false, 4000, 1, {0, 0, 0, 0});
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // AutoTask::AutoDelay(10000000),
        // StackTask(),
        DeployTask(),
        AutoTask::SyncTask(
            [](void) -> void {
                   
                    intake->Set_Intake(127);
                    robot->drive->Set_Point_Drive(100, 0, 1000, 0, 1, 1, true, 400, 1, {0, 60, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // slow downs while picking up cubes

        AutoTask::SyncTask(
            [](void) -> void {
                    intake->Set_Intake(200);
                    robot->drive->Set_Point_Drive(40, 0, 2100, 0, 1, 10, false, 2100, 1, {0, 0, 0, 0});
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(80);
                robot->drive->Set_Point_Drive(127, 180, 2400, 0, 1, 1, true, 400, 1, {0,0,0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(60, 90, 2100, 0, 1,3, true, 800, 1, {0,0, 0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // drive straight to reach the cubes

        // slow downs while picking up cubes
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(200);
                robot->drive->Set_Point_Drive(40, 0, 2650, 0, 2, 100, false, 2650, 1, {0, 0, 0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // extend angler a bit

        AutoTask::AutoDelay(200),

        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(80);
                robot->drive->Set_Point_Drive(80, 180, 2650, 0, 1, 0.8, false, 1050, 1, {0, 0, 0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }),
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-20*vision_indexer->Check_Object());
                robot->drive->Set_Point_Drive(0, 0, 2000, 80, 1, 1, false, 1000, 1, {0, 0, 0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }),

        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-20*vision_indexer->Check_Object());
                // angler->Smooth_Angler(1);
                robot->drive->Set_Point_Drive(40, 0, 1200, 0, 2, 2, false, 400, 1, {0, 0, 0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::AutoDelay(500),

        StackTask(),

        AutoTask::AutoDelay(600, true, [](void) -> void {
                robot->set_drive(0, 15, 0, 0);
        }),
        AutoTask::AutoDelay(200),

        // tower scoring starts here

        AutoTask::SyncTask([](void) -> void {
                // turning like 315 degrees to face tower
                robot->drive->Set_Point_Drive(60, 180, 900, 0, 1.5, 0.8); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::SyncTask([](void) -> void {
                angler->Auto_Angler(-2);

                // turning like 315 degrees to face tower
                robot->drive->Set_Point_Drive(0, 0, 2600, 100, 1.5, 0.8); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::AutoDelay(1000, true, [](void) -> void {
                robot->drive->Set_Drive(0,0,ultra_finder->Ultra_Angle(),0);
        }),
        // AutoTask::SyncTask([](void) -> void {
        //         // wall align
        //         robot->drive->Set_Point_Drive(35, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::SyncTask([](void) -> void {
                // driving to tower
                robot->drive->Set_Point_Drive(100, 0, 3000); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::SyncTask([](void) -> void {
                // driving close to tower and picking up cube
                robot->drive->Set_Point_Drive(20, 0, 500);
                // intaking cube
                intake->Set_Intake(100); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        // arm extends to 1400 height
        ArmTask(),

        // outtaking the cube
        AutoTask::AutoDelay(3000, true, [](void) -> void {
                intake->Set_Intake(-50);
        }),

        // driving back after dropping cube
        AutoTask::SyncTask([](void) -> void { robot->drive->Set_Point_Drive(60, 180, 1000); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        // lowering arm (this isnt acc needed)
        AutoTask::SyncTask([](void) -> void { arm->Increment_Arm(-1); }, [](void) -> bool { return ((arm->Get_Height() <= 100)); }),

        AutoTask::AutoDelay(10000000),
    });