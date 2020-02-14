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
AutoTask Auton::DeployTask = AutoTask::AutoDelay(
    900, true, [](void) -> void {
                                arm->Set_Target(300);
                                robot->drive->Set_Drive(0,-80,0,0);
                                intake->Set_Intake(-60); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Set_Intake(127); arm->Set_Target(0); });

AutoTask Auton::StackTask = AutoTask::SyncTask(
    [](void) -> void {
            // intake->Set_Intake(0);
            robot->set_drive(0, 0, 0, 0);
            angler->Smooth_Angler(1.5);
            angler->Override_Mode(-1);
    },
    [](void) -> bool { return ((angler->Get_Height() >= angler->_max_height)); },
    [](void) -> void {
            angler->Override_Mode(1);
    },
    [](void) -> void {
            angler->Override_Mode(0);
    });

AutoSequence *Auton::AT_Skills = AutoSequence::FromTasks(
    vector<AutoTask>{
        // AutoTask::SyncTask(
        //     [](void) -> void {
        //         intake->Set_Intake(200);
        //         robot->drive->Set_Point_Drive(40, 0, 5000, 0, 2, 0.8, false, 4000, 1, {0, 0, 0, 0});
        //     },
        //     [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // AutoTask::AutoDelay(10000000),
        DeployTask,

        AutoTask::SyncTask(
            [](void) -> void {
                    intake->Set_Intake(127);
                    robot->drive->Set_Point_Drive(127, 0, 1000, 0, 1, 1, false, 400, 1, {0, 60, 0, 0});
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
                robot->drive->Set_Point_Drive(127, 180, 2400, 0, 1, 1, false, 400, 1, {0,0,0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(60, 90, 1900, 0, 1,3, false, 800, 1, {0,0, 0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        // drive straight to reach the cubes

        // slow downs while picking up cubes
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(200);
                robot->drive->Set_Point_Drive(40, 0, 2650, 0, 2, 100, false, 2650, 1, {0, 0, 0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),
        // extend angler a bit

        AutoTask::AutoDelay(200),

        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(80);
                robot->drive->Set_Point_Drive(127, 180, 1100, 127, 2, 1, false, 1050, 1, {100, 0, 127, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }),
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-20*vision_indexer->Check_Object());
                robot->drive->Set_Point_Drive(127, 0, 1650, 127, 2, 2, false, 1000, 1, {0, 60, 0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }),

        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(-20*vision_indexer->Check_Object());
                angler->Smooth_Angler(1);
                robot->drive->Set_Point_Drive(60, 0, 700, 0, 2, 2, false, 400, 1, {0, 0, 0, 0}); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),

        AutoTask::AutoDelay(500),

        StackTask,

        AutoTask::AutoDelay(600, true, [](void) -> void {
                robot->set_drive(0, 20, 0, 0);
        }),
        AutoTask::AutoDelay(200),

        AutoTask::SyncTask([](void) -> void {
                // intake->Set_Intake(-40);
                robot->drive->Set_Point_Drive(127, 180, 800); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { robot->drive->Stop(); }),

        AutoTask::AutoDelay(10000000),
    });