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
#include "legacy_globals.hpp"

using namespace std;
using namespace Auton;

AutoSequence *invertedSmallSideAuto = AutoSequence::FromTasks(
    vector<AutoTask>{
        AutoTask::AutoDelay(500, false, [](void)->void{
            intake->Set_Intake(-80);
        }, [&](void)->void{
            return;
        }, [](void)->void{
            intake->Set_Intake(127);
        }),
        DeployTask(),
        AutoTask::SyncTask([](void) -> void {
                intake->Set_Intake(127);
                arm->Set_Target(0);
                robot->drive->Set_Point_Drive(60, 0, 1700); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        // AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),
        AutoTask::SyncTask(
            [](void) -> void { // diagonal drive
                intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(127, -140, 2800 + autonomous_increment);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(50, -90, 250, 0, 0.2);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(127, 0, 1000, 0, 1, false, 400, 3);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(60, 0, 2000);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::AutoDelay(200, true, [](void) -> void { intake->Set_Intake(127); }),

        Auton::InvertTurn90Deg(),
        AutoTask::SyncTask( //Diagonal
            [](void) -> void {
                intake->Set_Intake(127);
                robot->drive->Set_Point_Drive(127, -87, 3900, 0, 1.5, false, 100);
            },
            [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::SyncTask(
            [](void) -> void {
                intake->Set_Intake(0);
                robot->drive->Set_Point_Drive(40, 0, 200, 0, 3);
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

        StackTask(),
        AutoTask::AutoDelay(500),
        AutoTask::SyncTask([](void) -> void {
                    intake->Set_Intake(0);
                    robot->drive->Set_Point_Drive(40, 180, 500); }, [](void) -> bool { return (!robot->drive->get_running()); }, [](void) -> void { robot->drive->Reset_Point(); }, [](void) -> void { intake->Stop(); }),
        AutoTask::AutoDelay(10000000),
    });